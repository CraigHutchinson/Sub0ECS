#pragma once
/** Shared component types and system kernels used by every benchmark design.
 *
 * Kernels are identical to v1's published update_patterns benchmark (see the
 * master branch), so v1 numbers here are directly comparable with it.
 */

#include <cstdint>

#include "core.hpp"

namespace bench
{
    // Small entity
    struct Position { float x = 0.0f; float y = 0.0f; };
    struct Velocity { float dx = 0.0f; float dy = 0.0f; };

    // Medium entity adds
    struct Health   { float value = 100.0f; };
    struct Rotation { float angle = 0.0f; };
    struct Scale    { float value = 1.0f; };

    // Large entity adds
    struct Color { float r = 1.0f; float g = 1.0f; float b = 1.0f; float a = 1.0f; };
    struct Team  { std::int32_t id = 0; };
    struct Flags { std::int32_t value = 0; };

    // Rare marker (1% of entities) for sparse-query benchmarks
    struct Tag { std::int32_t value = 0; };

    // Churn component for add/remove benchmarks
    struct Frozen { std::int32_t ticks = 0; };

    namespace kernel
    {
        inline void updatePosition(Position& p, Velocity& v, float dt)
        {
            p.x += v.dx * dt;
            p.y += v.dy * dt;
            v.dy += 9.8f * dt;
            v.dx *= 0.99f;
            v.dy *= 0.99f;
            if (p.x < 0.0f) p.x += 1000.0f;
            if (p.x > 1000.0f) p.x -= 1000.0f;
            if (p.y < 0.0f) p.y += 1000.0f;
            if (p.y > 1000.0f) p.y -= 1000.0f;
        }

        inline void updateRotationHealth(Health& h, Rotation& r, float dt)
        {
            r.angle += 0.1f * dt;
            h.value -= 0.01f * dt;
        }

        inline void pulseScale(Scale& s, Color& c, float dt)
        {
            s.value *= (1.0f + 0.001f * dt);
            if (s.value > 2.0f) s.value = 1.0f;
            c.r = 0.5f + 0.5f * (s.value - 1.0f);
            c.g = 0.5f + 0.5f * (2.0f - s.value);
            c.b = 0.5f + 0.5f * ((s.value - 1.0f) * (2.0f - s.value));
        }
    } // namespace kernel

} // namespace bench
