#pragma once
/** Design-agnostic workloads. Every design is driven through the same small
 * adapter surface so the only variable is the storage/query model:
 *
 *   using Entity;  kName; kSupportsRemove; kSupportsDestroy;
 *   reserve(n); create(Cs...); each<Cs...>(f); find<C>(e);
 *   add<C>(e, c); remove<C>(e); destroy(e); commit();
 */

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

#include "components.hpp"

namespace bench
{
    enum class Pattern { Coherent, Fragmented };

    inline const char* toString(Pattern p) { return p == Pattern::Coherent ? "Coherent" : "Fragmented"; }

    inline constexpr float kDeltaTime = 1.0f / 60.0f;

    /** Deterministic RNG matching benchmarks/update_patterns/common.hpp */
    class Rng
    {
    public:
        explicit Rng(std::uint32_t seed = 42) : gen_(seed), dist_(-100.0f, 100.0f) {}
        float next() { return dist_(gen_); }

    private:
        std::mt19937 gen_;
        std::uniform_real_distribution<float> dist_;
    };

    /** Coherent: all Small (Position, Velocity).
     *  Fragmented: rotating Small / Medium(+Health,Rotation,Scale) / Large(+Color,Team,Flags). */
    template <typename W>
    typename W::Entity createOne(W& w, std::int64_t i, Pattern pattern, Rng& rng)
    {
        const Position p{ rng.next(), rng.next() };
        const Velocity v{ rng.next(), rng.next() };
        const int kind = pattern == Pattern::Coherent ? 0 : static_cast<int>(i % 3);
        switch (kind)
        {
        case 0: return w.create(p, v);
        case 1: return w.create(p, v, Health{}, Rotation{}, Scale{});
        default: return w.create(p, v, Health{}, Rotation{}, Scale{}, Color{}, Team{}, Flags{});
        }
    }

    template <typename W>
    std::vector<typename W::Entity> populate(W& w, std::int64_t n, Pattern pattern)
    {
        std::vector<typename W::Entity> out;
        out.reserve(static_cast<std::size_t>(n));
        w.reserve(static_cast<std::size_t>(n));
        Rng rng;
        for (std::int64_t i = 0; i < n; ++i) out.push_back(createOne(w, i, pattern, rng));
        w.commit();
        return out;
    }

    // ---- Systems -----------------------------------------------------------

    template <typename W>
    void systemIter1(W& w)
    {
        w.template each<Position>([](Position& p) { p.x += 1.0f; });
    }

    template <typename W>
    void systemPhysics(W& w)
    {
        w.template each<Position, Velocity>([](Position& p, Velocity& v) { kernel::updatePosition(p, v, kDeltaTime); });
    }

    template <typename W>
    void systemFrame3(W& w)
    {
        systemPhysics(w);
        w.template each<Health, Rotation>([](Health& h, Rotation& r) { kernel::updateRotationHealth(h, r, kDeltaTime); });
        w.template each<Scale, Color>([](Scale& s, Color& c) { kernel::pulseScale(s, c, kDeltaTime); });
    }

    /** 3-way query where the rarest component (Tag, 1%) should drive iteration. */
    template <typename W>
    void systemSparse(W& w)
    {
        w.template each<Position, Velocity, Tag>([](Position& p, Velocity& v, Tag& t) {
            kernel::updatePosition(p, v, kDeltaTime);
            ++t.value;
        });
    }

    // ---- Structural helpers -------------------------------------------------

    template <typename W>
    void tagEveryHundredth(W& w, const std::vector<typename W::Entity>& es)
    {
        for (std::size_t i = 0; i < es.size(); i += 100) w.add(es[i], Tag{});
        w.commit();
    }

    /** Add then remove Frozen on every 10th entity (world returns to same state). */
    template <typename W>
    void churnAddRemove(W& w, const std::vector<typename W::Entity>& es)
    {
        for (std::size_t i = 0; i < es.size(); i += 10) w.add(es[i], Frozen{ 1 });
        w.commit();
        for (std::size_t i = 0; i < es.size(); i += 10) w.template remove<Frozen>(es[i]);
        w.commit();
    }

    /** Add then remove a *queried* component (Tag) on every 10th entity.
     *  Unlike Frozen, this changes which systems match, so partitioned
     *  designs must move data. Precondition: no entity is tagged. */
    template <typename W>
    void churnTagAddRemove(W& w, const std::vector<typename W::Entity>& es)
    {
        for (std::size_t i = 0; i < es.size(); i += 10) w.add(es[i], Tag{});
        w.commit();
        for (std::size_t i = 0; i < es.size(); i += 10) w.template remove<Tag>(es[i]);
        w.commit();
    }

    /** Destroy every 10th entity (offset rotates) and create the same number of Small entities. */
    template <typename W>
    void churnDestroyCreate(W& w, std::vector<typename W::Entity>& es, std::size_t round, Rng& rng)
    {
        for (std::size_t i = round % 10; i < es.size(); i += 10) w.destroy(es[i]);
        for (std::size_t i = round % 10; i < es.size(); i += 10)
        {
            es[i] = w.create(Position{ rng.next(), rng.next() }, Velocity{ rng.next(), rng.next() });
        }
        w.commit();
    }

    /** Order-independent checksum using handle order (for conformance). */
    template <typename W>
    double checksum(W& w, const std::vector<typename W::Entity>& es)
    {
        double sum = 0.0;
        for (const auto& e : es)
        {
            if (auto* p = w.template find<Position>(e)) sum += static_cast<double>(p->x) * 3.0 + p->y;
            if (auto* v = w.template find<Velocity>(e)) sum += static_cast<double>(v->dx) * 5.0 + v->dy * 7.0;
            if (auto* h = w.template find<Health>(e)) sum += h->value * 11.0;
            if (auto* r = w.template find<Rotation>(e)) sum += r->angle * 29.0;
            if (auto* s = w.template find<Scale>(e)) sum += s->value * 13.0;
            if (auto* c = w.template find<Color>(e)) sum += c->r * 17.0 + c->g * 19.0;
            if (auto* t = w.template find<Tag>(e)) sum += t->value * 23.0;
            if (w.template find<Frozen>(e)) sum += 1000.0;
        }
        return sum;
    }

} // namespace bench
