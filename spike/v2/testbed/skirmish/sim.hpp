#pragma once
/** Skirmish — a deterministic, headless 4-team RTS simulation written against
 * the spike's storage-adapter surface, so every storage design runs the same
 * game (see README.md).
 *
 * Determinism across storage designs (which iterate in different orders):
 *   - every structural change goes through a command buffer, sorted by the
 *     stable Id component before it is applied (commit points);
 *   - spatial queries and float reductions visit entities in Id order;
 *   - randomness is a hash of (id, tick, salt), never a shared RNG stream.
 */

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

#include "../../common/query.hpp"
#include "../../fusion/access.hpp"
#include "../../fusion/executors.hpp"
#include "components.hpp"

namespace skirmish
{
    using spike::Query;
    namespace fusion = spike::fusion;

    inline constexpr int kMaxTeams = 4;

    // ---- deterministic hashing ------------------------------------------------
    inline std::uint64_t mix64(std::uint64_t x)
    {
        x += 0x9E3779B97F4A7C15ull;
        x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ull;
        x = (x ^ (x >> 27)) * 0x94D049BB133111EBull;
        return x ^ (x >> 31);
    }
    inline std::uint32_t hash3(std::uint32_t a, std::uint32_t b, std::uint32_t c)
    {
        return static_cast<std::uint32_t>(mix64((std::uint64_t(a) << 40) ^ (std::uint64_t(b) << 20) ^ c));
    }
    inline float hashUnit(std::uint32_t a, std::uint32_t b, std::uint32_t c)
    {
        return static_cast<float>(hash3(a, b, c) >> 8) * (1.0f / 16777216.0f);
    }

    // ---- uniform spatial grid, rebuilt each tick ----------------------------
    class Grid
    {
    public:
        struct Entry
        {
            std::uint32_t id;
            std::int32_t team;
            float x, y;
        };

        void reset(float size, unsigned workers = 1)
        {
            cells_ = std::max(1, static_cast<int>(std::ceil(size / kCellSize)));
            perWorker_.resize(workers);
            for (auto& v : perWorker_) v.clear();
        }

        void add(unsigned worker, const Entry& e) { perWorker_[worker].push_back(e); }

        /** Counting sort into cells, then Id order within each cell (determinism). */
        void finish()
        {
            staging_.clear();
            for (auto& v : perWorker_) staging_.insert(staging_.end(), v.begin(), v.end());
            const std::size_t n = static_cast<std::size_t>(cells_) * static_cast<std::size_t>(cells_);
            start_.assign(n + 1u, 0u);
            for (const Entry& e : staging_) ++start_[cellOf(e.x, e.y) + 1u];
            for (std::size_t i = 0; i < n; ++i) start_[i + 1u] += start_[i];
            entries_.resize(staging_.size());
            cursor_.assign(start_.begin(), start_.end() - 1);
            for (const Entry& e : staging_) entries_[cursor_[cellOf(e.x, e.y)]++] = e;
            for (std::size_t c = 0; c < n; ++c)
            {
                auto b = entries_.begin() + start_[c], en = entries_.begin() + start_[c + 1u];
                if (en - b > 1) std::sort(b, en, [](const Entry& l, const Entry& r) { return l.id < r.id; });
            }
        }

        /** Visit entries in cells overlapping the square of half-size r, in fixed order. */
        template <typename F>
        void forEachNear(float x, float y, float r, F&& f) const
        {
            const int x0 = coord(x - r), x1 = coord(x + r), y0 = coord(y - r), y1 = coord(y + r);
            for (int cy = y0; cy <= y1; ++cy)
            {
                for (int cx = x0; cx <= x1; ++cx)
                {
                    const std::size_t c = static_cast<std::size_t>(cy) * static_cast<std::size_t>(cells_) + static_cast<std::size_t>(cx);
                    for (std::uint32_t i = start_[c], e = start_[c + 1u]; i < e; ++i) f(entries_[i]);
                }
            }
        }

    private:
        int coord(float v) const { return std::clamp(static_cast<int>(v / kCellSize), 0, cells_ - 1); }
        std::size_t cellOf(float x, float y) const
        {
            return static_cast<std::size_t>(coord(y)) * static_cast<std::size_t>(cells_) + static_cast<std::size_t>(coord(x));
        }

        int cells_ = 1;
        std::vector<Entry> staging_, entries_;
        std::vector<std::vector<Entry>> perWorker_;
        std::vector<std::uint32_t> start_, cursor_;
    };

    // ---- movement systems: row-local, fusable (research/fusion.md) ---------

    struct Seek
    {
        using Query = spike::Query<MoveOrder, Position, Velocity, UnitType>;
        using Access = fusion::Access<fusion::Read<MoveOrder>, fusion::Read<Position>, fusion::Write<Velocity>, fusion::Read<UnitType>>;
        static constexpr bool kDeviceSafe = true;
        void operator()(MoveOrder& m, Position& p, Velocity& v, UnitType& t) const
        {
            const float dx = m.tx - p.x, dy = m.ty - p.y;
            const float d = std::sqrt(dx * dx + dy * dy);
            if (d > 0.5f)
            {
                const float s = kUnitStats[t.kind].speed / d;
                v.x += (dx * s - v.x) * 0.25f;
                v.y += (dy * s - v.y) * 0.25f;
            }
        }
    };

    /** Reads neighbours from the tick-start grid snapshot; writes only its own row. */
    struct Separation
    {
        const Grid* grid;
        using Query = spike::Query<Id, Position, Velocity, Team>;
        using Access = fusion::Access<fusion::Read<Id>, fusion::Read<Position>, fusion::Write<Velocity>, fusion::Read<Team>>;
        // not device-safe: reads the host-side Grid snapshot through a pointer
        void operator()(Id& id, Position& p, Velocity& v, Team&) const
        {
            float fx = 0.0f, fy = 0.0f;
            grid->forEachNear(p.x, p.y, kSeparation, [&](const Grid::Entry& e) {
                if (e.id == id.value) return;
                const float dx = p.x - e.x, dy = p.y - e.y;
                const float d2 = dx * dx + dy * dy;
                if (d2 < kSeparation * kSeparation && d2 > 1e-6f)
                {
                    const float d = std::sqrt(d2);
                    const float k = (kSeparation - d) / d;
                    fx += dx * k;
                    fy += dy * k;
                }
            });
            v.x += fx * 2.0f;
            v.y += fy * 2.0f;
        }
    };

    struct StunFreeze
    {
        using Query = spike::Query<Stunned, Velocity>;
        using Access = fusion::Access<fusion::Read<Stunned>, fusion::Write<Velocity>>;
        static constexpr bool kDeviceSafe = true;
        void operator()(Stunned&, Velocity& v) const { v.x = 0.0f; v.y = 0.0f; }
    };

    struct Integrate
    {
        using Query = spike::Query<Position, Velocity>;
        using Access = fusion::Access<fusion::Write<Position>, fusion::Read<Velocity>>;
        static constexpr bool kDeviceSafe = true;
        void operator()(Position& p, Velocity& v) const
        {
            p.x += v.x * kDt;
            p.y += v.y * kDt;
        }
    };

    struct Friction
    {
        using Query = spike::Query<Velocity, UnitType>;
        using Access = fusion::Access<fusion::Write<Velocity>, fusion::Read<UnitType>>;
        static constexpr bool kDeviceSafe = true;
        void operator()(Velocity& v, UnitType& t) const
        {
            v.x *= 0.92f;
            v.y *= 0.92f;
            const float maxS = kUnitStats[t.kind].speed;
            const float s2 = v.x * v.x + v.y * v.y;
            if (s2 > maxS * maxS)
            {
                const float k = maxS / std::sqrt(s2);
                v.x *= k;
                v.y *= k;
            }
        }
    };

    struct Bounds
    {
        float size;
        using Query = spike::Query<Position, Velocity>;
        using Access = fusion::Access<fusion::Write<Position>, fusion::Write<Velocity>>;
        static constexpr bool kDeviceSafe = true;
        void operator()(Position& p, Velocity& v) const
        {
            if (p.x < 0.0f) { p.x = 0.0f; v.x = -v.x * 0.5f; }
            if (p.y < 0.0f) { p.y = 0.0f; v.y = -v.y * 0.5f; }
            if (p.x > size) { p.x = size; v.x = -v.x * 0.5f; }
            if (p.y > size) { p.y = size; v.y = -v.y * 0.5f; }
        }
    };

    template <typename W, typename S, typename... Cs>
    void runPass(W& w, const S& s, Query<Cs...>)
    {
        w.template each<Cs...>([&](Cs&... cs) { s(cs...); });
    }

    // ---- simulation ------------------------------------------------------------

    struct Config
    {
        int teams = 4;
        int unitsPerTeam = 500;
        /** H8 threading: when set (and the world supports eachParallel), every
         *  system runs data-parallel on this pool with lock-step commit points. */
        spike::fusion::Parallel* pool = nullptr;
        bool fuseMovement = true;   ///< with a pool: one fused parallel pass vs one parallel pass per system
    };

    struct Stats
    {
        std::int64_t tick = 0;
        std::int64_t entities = 0;
        std::int64_t projectiles = 0;
        std::array<std::int32_t, kMaxTeams> units{};
        std::array<std::int32_t, kMaxTeams> resources{};
        std::array<std::int32_t, kMaxTeams> kills{};
        std::int32_t selected = 0;
        float selectedCx = 0.0f, selectedCy = 0.0f;
    };

    enum SystemIndex
    {
        kSysPopulation, kSysProduction, kSysGrid, kSysSelection, kSysAcquire, kSysCommander, kSysWorkers,
        kSysMovement, kSysArrive, kSysCombat, kSysProjectiles, kSysStatus, kSysDeath, kSysRegen, kSysCount
    };
    inline constexpr const char* kSystemNames[kSysCount] = { "population", "production", "grid", "selection", "acquire",
                                                             "commander", "workers", "movement", "arrive", "combat",
                                                             "projectiles", "status", "death", "regen" };

    /** Movement may be run by a pluggable runner (fusion planner + executor):
     *  Runner::run(world, systems...). void = built-in (sequential / fused). */
    template <typename W, bool Fused = false, typename Runner = void>
    class Sim
    {
    public:
        using Entity = typename W::Entity;

        Sim(W& world, Config cfg) : w_(world), cfg_(cfg)
        {
            const int total = cfg_.teams * cfg_.unitsPerTeam;
            mapSize_ = std::max(800.0f, 14.0f * std::sqrt(static_cast<float>(total)));
            byId_.push_back(Entity{});
            alive_.push_back(0);
            setup();
            w_.commit();
            population();   // so stats() are valid before the first tick
        }

        void tick()
        {
            timed(kSysPopulation, [&] { population(); });
            timed(kSysProduction, [&] { production(); apply(); });
            timed(kSysGrid, [&] { buildGrid(); });
            timed(kSysSelection, [&] { selection(); apply(); });
            timed(kSysAcquire, [&] { acquire(); apply(); });
            timed(kSysCommander, [&] { commander(); apply(); });
            timed(kSysWorkers, [&] { workers(); apply(); });
            timed(kSysMovement, [&] { movement(); });
            timed(kSysArrive, [&] { arrive(); apply(); });
            timed(kSysCombat, [&] { combat(); apply(); });
            timed(kSysProjectiles, [&] { projectiles(); apply(); });
            timed(kSysStatus, [&] { status(); apply(); });
            timed(kSysDeath, [&] { death(); apply(); });
            timed(kSysRegen, [&] { regen(); });
            ++tick_;
        }

        void enableTimings(bool on) { timing_ = on; }
        const std::array<double, kSysCount>& timings() const { return seconds_; }

        Stats stats()
        {
            Stats s;
            s.tick = tick_;
            s.projectiles = projectiles_;
            for (std::size_t i = 1; i < alive_.size(); ++i) s.entities += alive_[i];
            for (int t = 0; t < cfg_.teams; ++t)
            {
                s.units[t] = unitCount_[t];
                s.resources[t] = resources_[t];
                s.kills[t] = kills_[t];
            }
            s.selected = selectedCount_;
            s.selectedCx = selectedCx_;
            s.selectedCy = selectedCy_;
            return s;
        }

        /** Canonical-order state digest: identical across storage designs iff the games are identical. */
        double checksum()
        {
            double sum = 0.0;
            for (std::uint32_t id = 1; id < alive_.size(); ++id)
            {
                if (!alive_[id]) continue;
                const Entity e = byId_[id];
                const double k = static_cast<double>(id % 97u + 1u);
                if (auto* p = w_.template find<Position>(e)) sum += k * (p->x * 3.0 + p->y);
                if (auto* v = w_.template find<Velocity>(e)) sum += k * (v->x * 5.0 + v->y * 7.0);
                if (auto* h = w_.template find<Health>(e)) sum += k * h->hp * 11.0;
                if (auto* m = w_.template find<MoveOrder>(e)) sum += k * (m->tx + m->ty * 13.0);
                if (auto* t = w_.template find<Target>(e)) sum += k * t->id * 17.0;
                if (auto* c = w_.template find<Carrying>(e)) sum += k * c->amount * 19.0;
                if (auto* vt = w_.template find<Veteran>(e)) sum += k * vt->kills * 23.0;
                if (w_.template find<Stunned>(e)) sum += k * 29.0;
                if (w_.template find<Burning>(e)) sum += k * 31.0;
                if (w_.template find<Selected>(e)) sum += k * 37.0;
            }
            for (int t = 0; t < cfg_.teams; ++t) sum += resources_[t] * 41.0 + kills_[t] * 43.0;
            return sum;
        }

        float mapSize() const { return mapSize_; }
        W& world() { return w_; }

    private:
        // ---- command buffer (deferred structural changes) --------------------
        struct Spawn { std::uint32_t key; std::int32_t team, kind; float x, y; };
        struct SetMove { std::uint32_t id; float tx, ty; };
        struct Pair { std::uint32_t a, b; };
        struct Shoot { std::uint32_t shooter, target; std::int32_t effect; float x, y, vx, vy, damage; };
        struct Damage { std::uint32_t target, projectile, source; float amount; std::int32_t effect; };
        struct Deposit { std::uint32_t worker; std::int32_t team, amount; };

        struct Commands
        {
            std::vector<Spawn> spawn;
            std::vector<Pair> extract;     // (mine, worker)
            std::vector<Deposit> deposit;
            std::vector<Pair> addTarget;   // (id, target)
            std::vector<std::uint32_t> removeTarget, removeMove, addSelected, removeSelected, removeStunned,
                removeBurning, destroy;
            std::vector<SetMove> setMove;
            std::vector<Shoot> shoot;
            std::vector<Damage> damage;
            std::vector<Pair> kill;        // (victim, killer)
        };

        template <typename F>
        void timed(int sys, F&& f)
        {
            if (!timing_) { f(); return; }
            const auto t0 = std::chrono::steady_clock::now();
            f();
            seconds_[sys] += std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        }

        bool alive(std::uint32_t id) const { return id != 0 && id < alive_.size() && alive_[id]; }
        Entity handle(std::uint32_t id) const { return byId_[id]; }

        template <typename... Cs>
        std::uint32_t make(Cs... cs)
        {
            const std::uint32_t id = static_cast<std::uint32_t>(byId_.size());
            byId_.push_back(w_.create(Id{ id }, cs...));
            alive_.push_back(1);
            return id;
        }

        void kill(std::uint32_t id)
        {
            if (!alive(id)) return;
            if (w_.template find<Projectile>(handle(id))) --projectiles_;
            w_.destroy(handle(id));
            alive_[id] = 0;
        }

        // ---- world setup -------------------------------------------------------
        void setup()
        {
            const float L = mapSize_;
            const float half = L * 0.5f;
            for (int t = 0; t < cfg_.teams; ++t)
            {
                const float qx = (t % 2) * half, qy = (t / 2) * half;   // team quadrant origin
                const float cx = qx + half * 0.5f, cy = qy + half * 0.5f;
                hq_[t] = make(Position{ cx, cy }, Team{ t }, Building{ 0 }, Producer{}, Health{ 3000.f, 3000.f, 0 });
                resources_[t] = 500;
                for (int m = 0; m < 4; ++m)
                {
                    const float a = 1.5708f * m + 0.4f;
                    mines_[t].push_back(make(Position{ cx + 90.f * std::cos(a), cy + 90.f * std::sin(a) }, ResourceNode{ 1'000'000 }));
                }
                const int barracks = std::max(1, cfg_.unitsPerTeam / 250);
                for (int b = 0; b < barracks; ++b)
                {
                    const float bx = qx + 20.f + hashUnit(t, b, 1) * (half - 40.f);
                    const float by = qy + 20.f + hashUnit(t, b, 2) * (half - 40.f);
                    make(Position{ bx, by }, Team{ t }, Building{ 1 }, Producer{ hashUnit(t, b, 3), b }, Health{ 1500.f, 1500.f, 0 });
                }
                for (int u = 0; u < cfg_.unitsPerTeam; ++u)
                {
                    const float x = qx + 10.f + hashUnit(t, u, 4) * (half - 20.f);
                    const float y = qy + 10.f + hashUnit(t, u, 5) * (half - 20.f);
                    const int kind = (u % 7 == 0) ? kWorker : 1 + static_cast<int>(hash3(t, u, 6) % 3u);
                    spawnUnit(t, kind, x, y);
                }
            }
        }

        void spawnUnit(int team, int kind, float x, float y)
        {
            const UnitStats& s = kUnitStats[kind];
            const Health h{ s.maxHp, s.maxHp, 0 };
            if (kind == kWorker)
            {
                const std::uint32_t mine = mines_[team][nextMine_[team]++ % mines_[team].size()];
                make(Position{ x, y }, Velocity{}, Team{ team }, UnitType{ kind }, h, Worker{ hq_[team], mine, team });
            }
            else
            {
                make(Position{ x, y }, Velocity{}, Team{ team }, UnitType{ kind }, h, Weapon{});
            }
        }

        // ---- threading helpers (H8) ------------------------------------------
        unsigned workerCount() const { return cfg_.pool ? cfg_.pool->concurrency() : 1u; }

        /** Iterate a query; data-parallel on the pool when configured. f(worker, Cs&...). */
        template <typename... Cs, typename F>
        void forEach(F&& f)
        {
            if constexpr (requires { w_.template eachParallel<Cs...>(*cfg_.pool, f); })
            {
                if (cfg_.pool)
                {
                    w_.template eachParallel<Cs...>(*cfg_.pool, f);
                    return;
                }
            }
            w_.template each<Cs...>([&](Cs&... cs) { f(0u, cs...); });
        }

        // ---- systems -------------------------------------------------------------
        void population()
        {
            for (auto& p : popW_) p = {};
            forEach<Id, Team, UnitType>([&](unsigned tw, Id&, Team& t, UnitType& k) {
                ++popW_[tw].units[t.value];
                if (k.kind == kWorker) ++popW_[tw].workers[t.value];
            });
            workers_.fill(0);
            unitCount_.fill(0);
            for (const auto& p : popW_)
                for (int t = 0; t < kMaxTeams; ++t) { unitCount_[t] += p.units[t]; workers_[t] += p.workers[t]; }
        }

        void production()
        {
            forEach<Id, Building, Producer, Team, Position>([&](unsigned tw, Id& id, Building& b, Producer& pr, Team& t, Position& p) {
                pr.progress += (b.kind == 0 ? 0.5f : 1.0f) * kDt;
                if (pr.progress < 1.0f) return;
                pr.progress -= 1.0f;
                int kind = 1 + (pr.counter % 3);
                if (b.kind == 0) kind = (workers_[t.value] < cfg_.unitsPerTeam / 8) ? kWorker : kind;
                ++pr.counter;
                const float a = hashUnit(id.value, pr.counter, 7) * 6.2831f;
                cmds_[tw].spawn.push_back({ id.value, t.value, kind, p.x + 30.f * std::cos(a), p.y + 30.f * std::sin(a) });
            });
        }

        void buildGrid()
        {
            grid_.reset(mapSize_, workerCount());
            forEach<Id, Position, Team, Health>([&](unsigned tw, Id& id, Position& p, Team& t, Health&) {
                grid_.add(tw, { id.value, t.value, p.x, p.y });
            });
            grid_.finish();
        }

        /** Rare-component reduction every tick; reselect ~1% every 128 ticks. */
        void selection()
        {
            for (auto& v : selW_) v.clear();
            forEach<Id, Selected, Position>([&](unsigned tw, Id& id, Selected&, Position& p) { selW_[tw].push_back({ id.value, p.x, p.y }); });
            sel_.clear();
            for (const auto& v : selW_) sel_.insert(sel_.end(), v.begin(), v.end());
            std::sort(sel_.begin(), sel_.end(), [](const Sel& a, const Sel& b) { return a.id < b.id; });
            double sx = 0.0, sy = 0.0;
            for (const Sel& s : sel_) { sx += s.x; sy += s.y; }
            selectedCount_ = static_cast<std::int32_t>(sel_.size());
            selectedCx_ = sel_.empty() ? 0.f : static_cast<float>(sx / sel_.size());
            selectedCy_ = sel_.empty() ? 0.f : static_cast<float>(sy / sel_.size());
            if (tick_ % 128 != 0) return;
            for (const Sel& s : sel_) cmds_[0].removeSelected.push_back(s.id);   // sequential: buffer 0
            forEach<Id, Team, UnitType>([&](unsigned tw, Id& id, Team&, UnitType&) {
                if (hash3(id.value, static_cast<std::uint32_t>(tick_), 8) % 100u == 0u) cmds_[tw].addSelected.push_back(id.value);
            });
        }

        /** Non-row-local: spatial search over the grid; 1/8 of units per tick. */
        void acquire()
        {
            forEach<Id, Position, Team, Weapon, UnitType>([&](unsigned tw, Id& id, Position& p, Team& t, Weapon&, UnitType& k) {
                if ((id.value + static_cast<std::uint32_t>(tick_)) % 8u != 0u) return;
                if (w_.template find<Target>(handle(id.value))) return;
                const float r = kUnitStats[k.kind].range + kAcquireExtra;
                float bestD2 = r * r;
                std::uint32_t best = 0;
                grid_.forEachNear(p.x, p.y, r, [&](const Grid::Entry& e) {
                    if (e.team == t.value) return;
                    const float dx = e.x - p.x, dy = e.y - p.y, d2 = dx * dx + dy * dy;
                    if (d2 < bestD2 || (d2 == bestD2 && best != 0 && e.id < best)) { bestD2 = d2; best = e.id; }
                });
                if (best != 0) cmds_[tw].addTarget.push_back({ id.value, best });
            });
        }

        /** Every 64 ticks idle soldiers march on an enemy HQ. */
        void commander()
        {
            if (tick_ % 64 != 0) return;
            forEach<Id, Position, Team, Weapon, UnitType>([&](unsigned tw, Id& id, Position&, Team& t, Weapon&, UnitType&) {
                const Entity e = handle(id.value);
                if (w_.template find<Target>(e) || w_.template find<MoveOrder>(e)) return;
                const int enemy = (t.value + 1 + static_cast<int>(hash3(id.value, static_cast<std::uint32_t>(tick_), 9) % (cfg_.teams - 1))) % cfg_.teams;
                float tx = mapSize_ * 0.5f, ty = mapSize_ * 0.5f;
                if (alive(hq_[enemy]))
                {
                    const Position& hp = *w_.template find<Position>(handle(hq_[enemy]));
                    tx = hp.x; ty = hp.y;
                }
                tx += (hashUnit(id.value, static_cast<std::uint32_t>(tick_), 10) - 0.5f) * 120.f;
                ty += (hashUnit(id.value, static_cast<std::uint32_t>(tick_), 11) - 0.5f) * 120.f;
                cmds_[tw].setMove.push_back({ id.value, tx, ty });
            });
        }

        /** Harvest loop: random access to mine/HQ positions; Carrying add/remove churn. */
        void workers()
        {
            forEach<Id, Worker, Position>([&](unsigned tw, Id& id, Worker& wk, Position& p) {
                if (!alive(wk.mineId) || !alive(wk.homeId)) return;
                const Entity e = handle(id.value);
                const bool carrying = w_.template find<Carrying>(e) != nullptr;
                const Position& goal = *w_.template find<Position>(handle(carrying ? wk.homeId : wk.mineId));
                const float dx = goal.x - p.x, dy = goal.y - p.y, d2 = dx * dx + dy * dy;
                const float reach = carrying ? 40.f : 16.f;
                if (d2 < reach * reach)
                {
                    if (carrying) cmds_[tw].deposit.push_back({ id.value, wk.team, w_.template find<Carrying>(e)->amount });
                    else cmds_[tw].extract.push_back({ wk.mineId, id.value });
                }
                else if (!w_.template find<MoveOrder>(e))
                {
                    cmds_[tw].setMove.push_back({ id.value, goal.x, goal.y });
                }
            });
        }

        void movement()
        {
            const Seek seek{};
            const Separation separation{ &grid_ };
            const StunFreeze freeze{};
            const Integrate integrate{};
            const Friction friction{};
            const Bounds bounds{ mapSize_ };
            if constexpr (requires { w_.runFusedParallel(*cfg_.pool, integrate); })
            {
                if (cfg_.pool)
                {
                    if (cfg_.fuseMovement)
                        w_.runFusedParallel(*cfg_.pool, seek, separation, freeze, integrate, friction, bounds);
                    else
                    {
                        w_.runFusedParallel(*cfg_.pool, seek);
                        w_.runFusedParallel(*cfg_.pool, separation);
                        w_.runFusedParallel(*cfg_.pool, freeze);
                        w_.runFusedParallel(*cfg_.pool, integrate);
                        w_.runFusedParallel(*cfg_.pool, friction);
                        w_.runFusedParallel(*cfg_.pool, bounds);
                    }
                    return;
                }
            }
            if constexpr (!std::is_void_v<Runner>)
            {
                runner_.run(w_, seek, separation, freeze, integrate, friction, bounds);
            }
            else if constexpr (Fused && requires { w_.runFused(integrate); })
            {
                w_.runFused(seek, separation, freeze, integrate, friction, bounds);
            }
            else
            {
                runPass(w_, seek, Seek::Query{});
                runPass(w_, separation, Separation::Query{});
                runPass(w_, freeze, StunFreeze::Query{});
                runPass(w_, integrate, Integrate::Query{});
                runPass(w_, friction, Friction::Query{});
                runPass(w_, bounds, Bounds::Query{});
            }
        }

        void arrive()
        {
            forEach<Id, MoveOrder, Position>([&](unsigned tw, Id& id, MoveOrder& m, Position& p) {
                const float dx = m.tx - p.x, dy = m.ty - p.y;
                if (dx * dx + dy * dy < 64.f) cmds_[tw].removeMove.push_back(id.value);
            });
        }

        void combat()
        {
            forEach<Id, Position, Weapon, Target, Team, UnitType>(
                [&](unsigned tw, Id& id, Position& p, Weapon& wp, Target& tg, Team&, UnitType& k) {
                    const UnitStats& st = kUnitStats[k.kind];
                    wp.cooldown = std::max(0.0f, wp.cooldown - kDt);
                    if (!alive(tg.id)) { cmds_[tw].removeTarget.push_back(id.value); return; }
                    const Position& tp = *w_.template find<Position>(handle(tg.id));
                    const float dx = tp.x - p.x, dy = tp.y - p.y;
                    const float d = std::sqrt(dx * dx + dy * dy);
                    if (d > st.range + kAcquireExtra * 1.5f) { cmds_[tw].removeTarget.push_back(id.value); return; }
                    if (d > st.range) { cmds_[tw].setMove.push_back({ id.value, tp.x, tp.y }); return; }
                    if (w_.template find<MoveOrder>(handle(id.value))) cmds_[tw].removeMove.push_back(id.value);   // stand and fight
                    if (wp.cooldown > 0.0f || d < 1e-3f) return;
                    wp.cooldown = st.reload;
                    ++wp.shots;
                    const std::int32_t effect = (st.effect != kNoEffect && wp.shots % st.effectEvery == 0) ? st.effect : kNoEffect;
                    const float s = st.projectileSpeed / d;
                    cmds_[tw].shoot.push_back({ id.value, tg.id, effect, p.x, p.y, dx * s, dy * s, st.damage });
                });
        }

        /** High churn: projectiles home on a (randomly accessed) target, hit or expire. */
        void projectiles()
        {
            forEach<Id, Projectile, Position, Velocity>([&](unsigned tw, Id& id, Projectile& pr, Position& p, Velocity& v) {
                if (--pr.ttl <= 0) { cmds_[tw].destroy.push_back(id.value); return; }
                if (!alive(pr.targetId)) return;
                const Position& tp = *w_.template find<Position>(handle(pr.targetId));
                const float dx = tp.x - p.x, dy = tp.y - p.y, d2 = dx * dx + dy * dy;
                if (d2 < 36.f)
                {
                    cmds_[tw].damage.push_back({ pr.targetId, id.value, pr.ownerId, pr.damage, pr.effect });
                    cmds_[tw].destroy.push_back(id.value);
                    return;
                }
                const float speed = std::sqrt(v.x * v.x + v.y * v.y);
                const float inv = speed / std::sqrt(d2);
                v.x = dx * inv;
                v.y = dy * inv;
            });
        }

        void status()
        {
            forEach<Id, Stunned>([&](unsigned tw, Id& id, Stunned& s) {
                if (--s.ticks <= 0) cmds_[tw].removeStunned.push_back(id.value);
            });
            forEach<Id, Burning, Health>([&](unsigned tw, Id& id, Burning& b, Health& h) {
                h.hp -= 0.1f;
                if (--b.ticks <= 0) cmds_[tw].removeBurning.push_back(id.value);
            });
        }

        void death()
        {
            forEach<Id, Health, Team>([&](unsigned tw, Id& id, Health& h, Team&) {
                if (h.hp <= 0.0f) cmds_[tw].kill.push_back({ id.value, h.lastHitBy });
            });
        }

        void regen()
        {
            forEach<Health, UnitType>([&](unsigned, Health& h, UnitType& k) {
                h.hp = std::min(h.maxHp, h.hp + kUnitStats[k.kind].regen * kDt);
            });
        }

        // ---- commit point: apply commands in canonical order -----------------
        template <typename T, typename Key>
        static void sortBy(std::vector<T>& v, Key key)
        {
            std::stable_sort(v.begin(), v.end(), [&](const T& a, const T& b) { return key(a) < key(b); });
        }

        void setMove(std::uint32_t id, float tx, float ty)
        {
            if (!alive(id)) return;
            if (auto* m = w_.template find<MoveOrder>(handle(id))) { m->tx = tx; m->ty = ty; }
            else w_.add(handle(id), MoveOrder{ tx, ty });
        }

        template <typename C>
        void removeIf(std::uint32_t id)
        {
            if (alive(id) && w_.template find<C>(handle(id))) w_.template remove<C>(handle(id));
        }

        void apply()
        {
            mergeCommands();
            Commands& c_ = cmds_[0];
            auto byU32 = [](std::uint32_t x) { return x; };

            sortBy(c_.spawn, [](const Spawn& s) { return s.key; });
            for (const Spawn& s : c_.spawn)
            {
                const int cost = kUnitStats[s.kind].cost;
                if (unitCount_[s.team] >= cfg_.unitsPerTeam || resources_[s.team] < cost) continue;
                resources_[s.team] -= cost;
                ++unitCount_[s.team];
                spawnUnit(s.team, s.kind, std::clamp(s.x, 0.f, mapSize_), std::clamp(s.y, 0.f, mapSize_));
            }

            sortBy(c_.extract, [](const Pair& p) { return (std::uint64_t(p.a) << 32) | p.b; });
            for (const Pair& x : c_.extract)
            {
                if (!alive(x.a) || !alive(x.b)) continue;
                ResourceNode& node = *w_.template find<ResourceNode>(handle(x.a));
                const int take = std::min(10, node.amount);
                node.amount -= take;
                if (take <= 0) continue;
                w_.add(handle(x.b), Carrying{ take });
                const Worker& wk = *w_.template find<Worker>(handle(x.b));
                if (alive(wk.homeId))
                {
                    const Position& hp = *w_.template find<Position>(handle(wk.homeId));
                    setMove(x.b, hp.x, hp.y);
                }
            }
            sortBy(c_.deposit, [](const Deposit& d) { return d.worker; });
            for (const Deposit& d : c_.deposit)
            {
                if (!alive(d.worker)) continue;
                resources_[d.team] += d.amount;
                removeIf<Carrying>(d.worker);
            }

            sortBy(c_.addTarget, [](const Pair& p) { return p.a; });
            for (const Pair& p : c_.addTarget)
                if (alive(p.a) && alive(p.b) && !w_.template find<Target>(handle(p.a))) w_.add(handle(p.a), Target{ p.b });
            sortBy(c_.removeTarget, byU32);
            for (std::uint32_t id : c_.removeTarget) removeIf<Target>(id);

            sortBy(c_.setMove, [](const SetMove& s) { return s.id; });
            for (const SetMove& s : c_.setMove) setMove(s.id, s.tx, s.ty);
            sortBy(c_.removeMove, byU32);
            for (std::uint32_t id : c_.removeMove) removeIf<MoveOrder>(id);

            sortBy(c_.removeSelected, byU32);
            for (std::uint32_t id : c_.removeSelected) removeIf<Selected>(id);
            sortBy(c_.addSelected, byU32);
            for (std::uint32_t id : c_.addSelected)
                if (alive(id) && !w_.template find<Selected>(handle(id))) w_.add(handle(id), Selected{ 1 });

            sortBy(c_.shoot, [](const Shoot& s) { return s.shooter; });
            for (const Shoot& s : c_.shoot)
            {
                if (!alive(s.shooter)) continue;
                make(Position{ s.x, s.y }, Velocity{ s.vx, s.vy }, Projectile{ s.target, s.shooter, s.damage, 45, s.effect });
                ++projectiles_;
            }

            sortBy(c_.damage, [](const Damage& d) { return (std::uint64_t(d.target) << 32) | d.projectile; });
            for (const Damage& d : c_.damage)
            {
                if (!alive(d.target)) continue;
                const Entity e = handle(d.target);
                Health& h = *w_.template find<Health>(e);
                h.hp -= d.amount;
                h.lastHitBy = d.source;
                if (d.effect == kBurn)
                {
                    if (auto* b = w_.template find<Burning>(e)) b->ticks = 90;
                    else w_.add(e, Burning{ 90 });
                }
                else if (d.effect == kStun && w_.template find<Velocity>(e))
                {
                    if (auto* s = w_.template find<Stunned>(e)) s->ticks = 20;
                    else w_.add(e, Stunned{ 20 });
                }
            }

            sortBy(c_.removeStunned, byU32);
            for (std::uint32_t id : c_.removeStunned) removeIf<Stunned>(id);
            sortBy(c_.removeBurning, byU32);
            for (std::uint32_t id : c_.removeBurning) removeIf<Burning>(id);

            sortBy(c_.kill, [](const Pair& p) { return p.a; });
            for (const Pair& k : c_.kill)
            {
                if (!alive(k.a)) continue;
                if (alive(k.b))
                {
                    const Entity killer = handle(k.b);
                    if (auto* v = w_.template find<Veteran>(killer)) ++v->kills;
                    else w_.add(killer, Veteran{ 1 });
                    if (auto* t = w_.template find<Team>(killer)) ++kills_[t->value];
                }
                kill(k.a);
            }

            sortBy(c_.destroy, byU32);
            for (std::uint32_t id : c_.destroy) kill(id);

            clearKeepCapacity(c_);
            w_.commit();
        }

        /** Concatenate per-worker buffers into buffer 0. Every command key is
         *  unique within a system, so the later Id sort makes merge order irrelevant. */
        void mergeCommands()
        {
            Commands& d = cmds_[0];
            for (std::size_t w = 1; w < cmds_.size(); ++w)
            {
                Commands& s = cmds_[w];
                auto cat = [](auto& to, auto& from) { to.insert(to.end(), from.begin(), from.end()); from.clear(); };
                cat(d.spawn, s.spawn); cat(d.extract, s.extract); cat(d.deposit, s.deposit); cat(d.addTarget, s.addTarget);
                cat(d.removeTarget, s.removeTarget); cat(d.removeMove, s.removeMove); cat(d.addSelected, s.addSelected);
                cat(d.removeSelected, s.removeSelected); cat(d.removeStunned, s.removeStunned);
                cat(d.removeBurning, s.removeBurning); cat(d.destroy, s.destroy); cat(d.setMove, s.setMove);
                cat(d.shoot, s.shoot); cat(d.damage, s.damage); cat(d.kill, s.kill);
            }
        }

        static void clearKeepCapacity(Commands& c_)
        {
            c_.spawn.clear(); c_.extract.clear(); c_.deposit.clear(); c_.addTarget.clear(); c_.removeTarget.clear();
            c_.removeMove.clear(); c_.addSelected.clear(); c_.removeSelected.clear(); c_.removeStunned.clear();
            c_.removeBurning.clear(); c_.destroy.clear(); c_.setMove.clear(); c_.shoot.clear(); c_.damage.clear();
            c_.kill.clear();
        }

        struct Sel { std::uint32_t id; float x, y; };

        W& w_;
        std::conditional_t<std::is_void_v<Runner>, char, Runner> runner_{};
        Config cfg_;
        float mapSize_ = 0.0f;
        std::int64_t tick_ = 0;
        std::int64_t projectiles_ = 0;
        std::vector<Entity> byId_;
        std::vector<std::uint8_t> alive_;
        std::array<std::uint32_t, kMaxTeams> hq_{};
        std::array<std::vector<std::uint32_t>, kMaxTeams> mines_{};
        std::array<std::uint32_t, kMaxTeams> nextMine_{};
        std::array<std::int32_t, kMaxTeams> resources_{}, kills_{}, unitCount_{}, workers_{};
        std::int32_t selectedCount_ = 0;
        float selectedCx_ = 0.0f, selectedCy_ = 0.0f;
        std::vector<Sel> sel_;
        Grid grid_;
        struct PopCount { std::array<std::int32_t, kMaxTeams> units{}, workers{}; };
        std::vector<Commands> cmds_ = std::vector<Commands>(workerCount());
        std::vector<PopCount> popW_ = std::vector<PopCount>(workerCount());
        std::vector<std::vector<Sel>> selW_ = std::vector<std::vector<Sel>>(workerCount());
        bool timing_ = false;
        std::array<double, kSysCount> seconds_{};
    };

} // namespace skirmish
