#pragma once
/** System definitions for the fusion spike (H7).
 *
 * A system is a row-local callable with a declared query:
 *   struct S { using Query = spike::Query<Cs...>; void operator()(Cs&...) const; };
 * "Row-local" means the kernel reads/writes only the current entity's
 * components — the property that makes fusion legal (research §4.C4,
 * design-review-pre-h2.md, fusion.md).
 */

#include "components.hpp"
#include "query.hpp"
#include "scenarios.hpp"

namespace spike
{
    // ---- FusionFrame: Update2's kernel split into three systems that share
    //      Position/Velocity (fusion should help), plus one on other columns.
    //      Applied in order per row they equal kernel::updatePosition.

    struct Integrate
    {
        using Query = spike::Query<Position, Velocity>;
        void operator()(Position& p, Velocity& v) const
        {
            p.x += v.dx * kDeltaTime;
            p.y += v.dy * kDeltaTime;
        }
    };

    struct Forces
    {
        using Query = spike::Query<Position, Velocity>;
        void operator()(Position&, Velocity& v) const
        {
            v.dy += 9.8f * kDeltaTime;
            v.dx *= 0.99f;
            v.dy *= 0.99f;
        }
    };

    struct Wrap
    {
        using Query = spike::Query<Position, Velocity>;
        void operator()(Position& p, Velocity&) const
        {
            if (p.x < 0.0f) p.x += 1000.0f;
            if (p.x > 1000.0f) p.x -= 1000.0f;
            if (p.y < 0.0f) p.y += 1000.0f;
            if (p.y > 1000.0f) p.y -= 1000.0f;
        }
    };

    // ---- Frame3 as systems (disjoint columns: fusion saves passes, not bytes)

    struct PhysicsSys
    {
        using Query = spike::Query<Position, Velocity>;
        void operator()(Position& p, Velocity& v) const { kernel::updatePosition(p, v, kDeltaTime); }
    };

    struct RotHealthSys
    {
        using Query = spike::Query<Health, Rotation>;
        void operator()(Health& h, Rotation& r) const { kernel::updateRotationHealth(h, r, kDeltaTime); }
    };

    struct PulseSys
    {
        using Query = spike::Query<Scale, Color>;
        void operator()(Scale& s, Color& c) const { kernel::pulseScale(s, c, kDeltaTime); }
    };

    /** Unfused reference: one full pass per system, in order (works for every design). */
    template <typename W, typename S, typename... Cs>
    void runPass(W& w, const S& s, Query<Cs...>)
    {
        w.template each<Cs...>([&](Cs&... cs) { s(cs...); });
    }

    template <typename W, typename... Systems>
    void runSequential(W& w, const Systems&... systems)
    {
        (runPass(w, systems, typename Systems::Query{}), ...);
    }

    template <typename W>
    void systemFusionFrame(W& w) { runSequential(w, Integrate{}, Forces{}, Wrap{}, RotHealthSys{}); }

    template <typename W>
    void systemFrame3Seq(W& w) { runSequential(w, PhysicsSys{}, RotHealthSys{}, PulseSys{}); }

    template <typename W>
    void systemFusionFrameFused(W& w) { w.runFused(Integrate{}, Forces{}, Wrap{}, RotHealthSys{}); }

    template <typename W>
    void systemFrame3Fused(W& w) { w.runFused(PhysicsSys{}, RotHealthSys{}, PulseSys{}); }

    /** Sharing-driven grouping (the planner rule under test): fuse only the
     *  systems that share columns; RotHealth shares none, so it runs alone. */
    template <typename W>
    void systemFusionFrameGrouped(W& w)
    {
        w.runFused(Integrate{}, Forces{}, Wrap{});
        w.runFused(RotHealthSys{});
    }

    /** Upper bound for FusionFrame: the hand-merged kernel (Update2) + RotHealth. */
    template <typename W>
    void systemFusionFrameHand(W& w)
    {
        systemPhysics(w);
        runSequential(w, RotHealthSys{});
    }

} // namespace spike
