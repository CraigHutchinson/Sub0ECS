#pragma once
/** Design A — Sparse set per component (EnTT-style).
 *
 * Storage: per component a sparse index array (entity slot -> dense row) plus
 *          packed dense arrays of entities and components.
 * Query:   drive from the smallest pool, O(1) membership test in the others.
 * Mutation: add/remove O(1) (swap-and-pop), destroy O(#pools).
 * Registry: runtime, per-world vector indexed by process-wide typeId<T>().
 *
 * Spike simplification: sparse arrays are flat (not paged), so a rare
 * component on a high entity index costs 4 bytes * max-index.
 */

#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>

#include "../common/components.hpp"
#include <sub0ecs/detail/hints.hpp>
#include <sub0ecs/entity.hpp>

namespace bench::sparse
{
    class PoolBase
    {
    public:
        virtual ~PoolBase() = default;
        virtual void removeIfPresent(Entity e) = 0;
        virtual std::size_t size() const = 0;
    };

    template <typename T>
    class Pool final : public PoolBase
    {
    public:
        static constexpr std::uint32_t kNull = ~0u;

        bool contains(Entity e) const
        {
            const std::uint32_t i = e.index();
            return i < sparse_.size() && sparse_[i] != kNull && dense_[sparse_[i]] == e;
        }

        T* find(Entity e) { return contains(e) ? &data_[sparse_[e.index()]] : nullptr; }
        T& getUnchecked(Entity e) { return data_[sparse_[e.index()]]; }

        void emplace(Entity e, T value)
        {
            const std::uint32_t i = e.index();
            if (i >= sparse_.size())
            {
                sparse_.resize(i + 1u, kNull);
            }
            sparse_[i] = static_cast<std::uint32_t>(dense_.size());
            dense_.push_back(e);
            data_.push_back(std::move(value));
        }

        void remove(Entity e)
        {
            const std::uint32_t row = sparse_[e.index()];
            const Entity last = dense_.back();
            dense_[row] = last;
            data_[row] = std::move(data_.back());
            sparse_[last.index()] = row;
            sparse_[e.index()] = kNull;
            dense_.pop_back();
            data_.pop_back();
        }

        void removeIfPresent(Entity e) override
        {
            if (contains(e)) remove(e);
        }

        std::size_t size() const override { return dense_.size(); }
        const Entity* entities() const { return dense_.data(); }
        T* data() { return data_.data(); }

    private:
        std::vector<std::uint32_t> sparse_;
        std::vector<Entity> dense_;
        std::vector<T> data_;
    };

    class World
    {
    public:
        using Entity = bench::Entity;
        static constexpr const char* kName = "SparseSet";
        static constexpr bool kSupportsRemove = true;
        static constexpr bool kSupportsDestroy = true;

        void reserve(std::size_t n) { entities_.reserve(n); }

        template <typename... Cs>
        Entity create(Cs... cs)
        {
            const Entity e = entities_.create();
            (pool<Cs>().emplace(e, std::move(cs)), ...);
            return e;
        }

        template <typename... Cs, typename F>
        void each(F&& f)
        {
            std::tuple<Pool<Cs>*...> pools{ findPool<Cs>()... };
            if ((!std::get<Pool<Cs>*>(pools) || ...)) return;

            if constexpr (sizeof...(Cs) == 1)
            {
                auto& p = *std::get<0>(pools);
                auto* d = p.data();
                for (std::size_t i = 0, n = p.size(); i < n; ++i) SUB0ECS_FLATTEN_CALLS f(d[i]);
            }
            else
            {
                // Pick smallest pool as the leader (runtime), dispatch to compile-time index.
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
            return p ? p->find(e) : nullptr;
        }

        template <typename C>
        void add(Entity e, C c) { pool<C>().emplace(e, std::move(c)); }

        template <typename C>
        void remove(Entity e) { pool<C>().remove(e); }

        void destroy(Entity e)
        {
            for (auto& p : pools_)
            {
                if (p) p->removeIfPresent(e);
            }
            entities_.release(e);
        }

        void commit() {}

    private:
        template <typename Pools, typename F, std::size_t... Is>
        void dispatchLeader(Pools& pools, std::size_t leader, F& f, std::index_sequence<Is...>)
        {
            (void)((leader == Is ? (iterateLedBy<Is>(pools, f), 0) : 0), ...);
        }

        template <std::size_t L, typename Pools, typename F>
        void iterateLedBy(Pools& pools, F& f)
        {
            iterateLedBy<L>(pools, f, std::make_index_sequence<std::tuple_size_v<Pools>>{});
        }

        template <std::size_t L, typename Pools, typename F, std::size_t... Is>
        void iterateLedBy(Pools& pools, F& f, std::index_sequence<Is...>)
        {
            auto& lead = *std::get<L>(pools);
            const Entity* ents = lead.entities();
            auto* leadData = lead.data();
            for (std::size_t i = 0, n = lead.size(); i < n; ++i)
            {
                const Entity e = ents[i];
                if (((Is == L || std::get<Is>(pools)->contains(e)) && ...))
                {
                    SUB0ECS_FLATTEN_CALLS f(access<Is, L>(pools, leadData, i, e)...);
                }
            }
        }

        template <std::size_t I, std::size_t L, typename Pools, typename LeadT>
        static auto& access(Pools& pools, LeadT* leadData, std::size_t row, Entity e)
        {
            if constexpr (I == L) return leadData[row];
            else return std::get<I>(pools)->getUnchecked(e);
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

        EntityAllocator entities_;
        std::vector<std::unique_ptr<PoolBase>> pools_;
    };

} // namespace bench::sparse
