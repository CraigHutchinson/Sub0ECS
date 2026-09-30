#pragma once
/** Design C — "v1-evolved": sorted id vectors, fixed.
 *
 * Keeps v1's data model (per component: sorted entity-id vector + parallel
 * component vector) to test whether the model itself is viable for v2 once
 * the known v1 costs are removed:
 *   - raw pointer/index iteration (no tuple-of-iterators, no .at() bounds checks)
 *   - leader = smallest collection; others gallop (exponential + binary search)
 *   - per-world runtime registry (no static 32-world table)
 *   - remove/destroy supported via deferred, batched structural changes:
 *     out-of-order inserts and removals are staged and merged in one O(n)
 *     pass at the next commit()/query, keeping the arrays sorted.
 * Ids are monotonic (never recycled) so in-order creation is an append.
 */

#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>

#include "../common/components.hpp"
#include <sub0ecs/entity.hpp>

namespace bench::sorted
{
    using Id = std::uint32_t;

    class PoolBase
    {
    public:
        virtual ~PoolBase() = default;
        virtual void stageRemove(Id id) = 0;
        virtual void flush() = 0;
        virtual std::size_t size() const = 0;
    };

    template <typename T>
    class Pool final : public PoolBase
    {
    public:
        void add(Id id, T value)
        {
            if (!dirty() && (ids_.empty() || ids_.back() < id))
            {
                ids_.push_back(id);
                data_.push_back(std::move(value));
            }
            else
            {
                stagedAdd_.emplace_back(id, std::move(value));
            }
        }

        void stageRemove(Id id) override { stagedRemove_.push_back(id); }

        bool dirty() const { return !stagedAdd_.empty() || !stagedRemove_.empty(); }

        void flush() override
        {
            if (!stagedRemove_.empty())
            {
                std::sort(stagedRemove_.begin(), stagedRemove_.end());
                std::size_t w = 0, r = 0;
                auto rm = stagedRemove_.begin();
                for (; r < ids_.size(); ++r)
                {
                    while (rm != stagedRemove_.end() && *rm < ids_[r]) ++rm;
                    if (rm != stagedRemove_.end() && *rm == ids_[r]) continue;
                    if (w != r)
                    {
                        ids_[w] = ids_[r];
                        data_[w] = std::move(data_[r]);
                    }
                    ++w;
                }
                ids_.resize(w);
                data_.resize(w);
                stagedRemove_.clear();
            }
            if (!stagedAdd_.empty())
            {
                std::sort(stagedAdd_.begin(), stagedAdd_.end(),
                          [](const auto& a, const auto& b) { return a.first < b.first; });
                // Backward in-place merge into the grown arrays.
                std::size_t i = ids_.size(), j = stagedAdd_.size();
                ids_.resize(i + j);
                data_.resize(i + j);
                std::size_t k = i + j;
                while (j > 0)
                {
                    if (i > 0 && ids_[i - 1] > stagedAdd_[j - 1].first)
                    {
                        --i; --k;
                        ids_[k] = ids_[i];
                        data_[k] = std::move(data_[i]);
                    }
                    else
                    {
                        --j; --k;
                        ids_[k] = stagedAdd_[j].first;
                        data_[k] = std::move(stagedAdd_[j].second);
                    }
                }
                stagedAdd_.clear();
            }
        }

        T* find(Id id)
        {
            auto it = std::lower_bound(ids_.begin(), ids_.end(), id);
            return (it != ids_.end() && *it == id) ? &data_[static_cast<std::size_t>(it - ids_.begin())] : nullptr;
        }

        std::size_t size() const override { return ids_.size(); }
        const Id* ids() const { return ids_.data(); }
        T* data() { return data_.data(); }

    private:
        std::vector<Id> ids_;
        std::vector<T> data_;
        std::vector<std::pair<Id, T>> stagedAdd_;
        std::vector<Id> stagedRemove_;
    };

    /** Advance cursor so ids[cursor] >= target (exponential then binary search). */
    inline std::size_t gallop(const Id* ids, std::size_t cursor, std::size_t n, Id target)
    {
        if (cursor >= n || ids[cursor] >= target) return cursor;
        std::size_t step = 1, lo = cursor, hi = cursor + 1;
        while (hi < n && ids[hi] < target)
        {
            lo = hi;
            step <<= 1;
            hi = cursor + step;
        }
        if (hi > n) hi = n;
        return static_cast<std::size_t>(std::lower_bound(ids + lo, ids + hi, target) - ids);
    }

    class World
    {
    public:
        using Entity = Id;
        static constexpr const char* kName = "SortedSoA";
        static constexpr bool kSupportsRemove = true;
        static constexpr bool kSupportsDestroy = true;

        void reserve(std::size_t) {}

        template <typename... Cs>
        Entity create(Cs... cs)
        {
            const Id id = next_++;
            (pool<Cs>().add(id, std::move(cs)), ...);
            return id;
        }

        template <typename... Cs, typename F>
        void each(F&& f)
        {
            std::tuple<Pool<Cs>*...> pools{ findPool<Cs>()... };
            if (((std::get<Pool<Cs>*>(pools) == nullptr) || ...)) return;
            (flushIfDirty(*std::get<Pool<Cs>*>(pools)), ...);

            if constexpr (sizeof...(Cs) == 1)
            {
                auto& p = *std::get<0>(pools);
                auto* d = p.data();
                for (std::size_t i = 0, n = p.size(); i < n; ++i) f(d[i]);
            }
            else
            {
                const std::size_t sizes[] = { std::get<Pool<Cs>*>(pools)->size()... };
                const std::size_t leader =
                    static_cast<std::size_t>(std::min_element(std::begin(sizes), std::end(sizes)) - sizes);
                dispatchLeader(pools, leader, f, std::index_sequence_for<Cs...>{});
            }
        }

        template <typename C>
        C* find(Entity e)
        {
            Pool<C>* p = findPool<C>();
            if (!p) return nullptr;
            flushIfDirty(*p);
            return p->find(e);
        }

        template <typename C>
        void add(Entity e, C c) { pool<C>().add(e, std::move(c)); }

        template <typename C>
        void remove(Entity e) { pool<C>().stageRemove(e); }

        void destroy(Entity e)
        {
            for (auto& p : pools_)
            {
                if (p) p->stageRemove(e);   // absent ids are ignored by the merge
            }
        }

        /** Structural sync point: apply all staged adds/removes. */
        void commit()
        {
            for (auto& p : pools_)
            {
                if (p) p->flush();
            }
        }

    private:
        template <typename P>
        static void flushIfDirty(P& p)
        {
            if (p.dirty()) p.flush();
        }

        template <typename Pools, typename F, std::size_t... Is>
        void dispatchLeader(Pools& pools, std::size_t leader, F& f, std::index_sequence<Is...> seq)
        {
            (void)((leader == Is ? (iterateLedBy<Is>(pools, f, seq), 0) : 0), ...);
        }

        template <std::size_t L, typename Pools, typename F, std::size_t... Is>
        void iterateLedBy(Pools& pools, F& f, std::index_sequence<Is...>)
        {
            constexpr std::size_t N = sizeof...(Is);
            const Id* ids[N] = { std::get<Is>(pools)->ids()... };
            const std::size_t sizes[N] = { std::get<Is>(pools)->size()... };
            std::size_t cursor[N] = {};

            auto data = std::make_tuple(std::get<Is>(pools)->data()...);

            const std::size_t n = sizes[L];
            for (std::size_t i = 0; i < n; ++i)
            {
                const Id target = ids[L][i];
                cursor[L] = i;
                // Returns false when this follower is exhausted or does not hold target.
                // Fast path: the next id is usually the target (dense overlap) so step
                // linearly once before falling back to galloping.
                auto seek = [&](std::size_t k) {
                    std::size_t c = cursor[k];
                    const Id* p = ids[k];
                    if (c < sizes[k] && p[c] < target)
                    {
                        ++c;
                        if (c < sizes[k] && p[c] < target) c = gallop(p, c, sizes[k], target);
                    }
                    cursor[k] = c;
                    return c < sizes[k] && p[c] == target;
                };
                if (((Is == L || seek(Is)) && ...))
                {
                    f(std::get<Is>(data)[cursor[Is]]...);
                }
                else if (((Is != L && cursor[Is] >= sizes[Is]) || ...))
                {
                    return;  // a follower ran out: no further matches possible
                }
            }
        }

        template <typename C>
        Pool<C>* findPool()
        {
            const std::uint32_t id = typeId<C>();
            return id < pools_.size() ? static_cast<Pool<C>*>(pools_[id].get()) : nullptr;
        }

        template <typename C>
        Pool<C>& pool()
        {
            const std::uint32_t id = typeId<C>();
            if (id >= pools_.size()) pools_.resize(id + 1u);
            if (!pools_[id]) pools_[id] = std::make_unique<Pool<C>>();
            return static_cast<Pool<C>&>(*pools_[id]);
        }

        Id next_ = 0;
        std::vector<std::unique_ptr<PoolBase>> pools_;
    };

} // namespace bench::sorted
