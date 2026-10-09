#pragma once
/** The bars: this workload written by hand, with no ECS at all.
 *
 * An ECS is a general mechanism; these are what it is measured against. Both
 * know the workload in advance: three entity shapes (Small, Medium, Large),
 * created in a fixed order, never changing shape. Neither supports queries,
 * adding or removing components, or destroying entities.
 *
 *   Plain   "HandWritten": one array per component per shape and a plain loop
 *           calling the shared kernel. What a programmer writes first, and the
 *           paired baseline of the iteration scenarios.
 *   Tuned   "HandTuned": one array per field, branch-free kernels, explicit AVX2
 *           where the build targets it. The ceiling: what the hardware allows
 *           for this arithmetic.
 *
 * Both must reproduce the shared kernels bit for bit (test_design_conformance):
 * Tuned uses the same operations in the same order, with no fused multiply-add.
 */

#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>
#include <vector>

#if defined(__AVX2__)
#    include <immintrin.h>
#endif

#include "../common/components.hpp"
#include "../common/scenarios.hpp"

namespace bench::hand
{
    /** Where entity i of populate() lives: its shape and its row in that shape's arrays. */
    struct Place
    {
        int shape;         // 0 Small, 1 Medium, 2 Large
        std::size_t row;
    };
    inline Place placeOf(std::size_t i, Pattern pattern)
    {
        return pattern == Pattern::Coherent ? Place{ 0, i } : Place{ static_cast<int>(i % 3), i / 3 };
    }

    /** Handle for random access: shape in the low two bits, row above. */
    inline std::uint32_t handleOf(Place p) { return static_cast<std::uint32_t>(p.row << 2) | static_cast<std::uint32_t>(p.shape); }

    // ---- HandWritten ---------------------------------------------------------

    class Plain
    {
    public:
        static constexpr const char* kName = "HandWritten";

        /** The same entities, values and order as populate() gives every design. */
        Plain(std::int64_t n, Pattern pattern)
        {
            Rng rng;
            for (std::int64_t i = 0; i < n; ++i)
            {
                const Position p{ rng.next(), rng.next() };
                const Velocity v{ rng.next(), rng.next() };
                Shape& s = shapes_[placeOf(static_cast<std::size_t>(i), pattern).shape];
                s.position.push_back(p);
                s.velocity.push_back(v);
            }
            for (int shape = 1; shape < 3; ++shape)
            {
                const std::size_t rows = shapes_[shape].position.size();
                shapes_[shape].health.resize(rows);
                shapes_[shape].rotation.resize(rows);
                shapes_[shape].scale.resize(rows);
            }
            shapes_[2].color.resize(shapes_[2].position.size());
        }

        void iter1()
        {
            for (Shape& s : shapes_)
                for (Position& p : s.position) p.x += 1.0f;
        }

        void update2()
        {
            for (Shape& s : shapes_)
            {
                Position* position = s.position.data();
                Velocity* velocity = s.velocity.data();
                for (std::size_t i = 0, n = s.position.size(); i < n; ++i) kernel::updatePosition(position[i], velocity[i], kDeltaTime);
            }
        }

        void frame3()
        {
            update2();
            for (int shape = 1; shape < 3; ++shape)
            {
                Shape& s = shapes_[shape];
                for (std::size_t i = 0, n = s.health.size(); i < n; ++i) kernel::updateRotationHealth(s.health[i], s.rotation[i], kDeltaTime);
            }
            Shape& large = shapes_[2];
            for (std::size_t i = 0, n = large.color.size(); i < n; ++i) kernel::pulseScale(large.scale[i], large.color[i], kDeltaTime);
        }

        float randomGet(const std::vector<std::uint32_t>& handles) const
        {
            float sum = 0.0f;
            for (std::uint32_t h : handles) sum += shapes_[h & 3u].velocity[h >> 2].dx;
            return sum;
        }

        /** bench::checksum() over the entities in creation order. */
        double checksum(std::int64_t n, Pattern pattern) const
        {
            double sum = 0.0;
            for (std::int64_t i = 0; i < n; ++i)
            {
                const Place at = placeOf(static_cast<std::size_t>(i), pattern);
                const Shape& s = shapes_[at.shape];
                sum += static_cast<double>(s.position[at.row].x) * 3.0 + s.position[at.row].y;
                sum += static_cast<double>(s.velocity[at.row].dx) * 5.0 + s.velocity[at.row].dy * 7.0;
                if (at.shape >= 1)
                {
                    sum += s.health[at.row].value * 11.0;
                    sum += s.rotation[at.row].angle * 29.0;
                    sum += s.scale[at.row].value * 13.0;
                }
                if (at.shape == 2) sum += s.color[at.row].r * 17.0 + s.color[at.row].g * 19.0;
            }
            return sum;
        }

    private:
        struct Shape
        {
            std::vector<Position> position;
            std::vector<Velocity> velocity;
            std::vector<Health> health;       // Medium and Large
            std::vector<Rotation> rotation;   // Medium and Large
            std::vector<Scale> scale;         // Medium and Large
            std::vector<Color> color;         // Large
        };
        Shape shapes_[3];
    };

    // ---- HandTuned -----------------------------------------------------------

    /** One float per row, 64-byte aligned, padded to a whole number of 8-wide
     *  vectors so loops need no scalar tail (the padding is computed and ignored). */
    class Field
    {
    public:
        void assign(std::size_t rows, float value)
        {
            rows_ = rows;
            const std::size_t padded = (rows + 7u) & ~std::size_t{ 7 };
            data_.reset(padded ? static_cast<float*>(::operator new(padded * sizeof(float), std::align_val_t{ 64 })) : nullptr);
            for (std::size_t i = 0; i < padded; ++i) data_[i] = value;
        }
        float* data() { return data_.get(); }
        float operator[](std::size_t i) const { return data_[i]; }
        float& operator[](std::size_t i) { return data_[i]; }
        std::size_t rows() const { return rows_; }
        std::size_t padded() const { return (rows_ + 7u) & ~std::size_t{ 7 }; }

    private:
        struct Free
        {
            void operator()(float* p) const { ::operator delete(p, std::align_val_t{ 64 }); }
        };
        std::unique_ptr<float[], Free> data_;
        std::size_t rows_ = 0;
    };

#if defined(_MSC_VER) && !defined(__clang__)
#    define SUB0ECS_BENCH_RESTRICT __restrict
#else
#    define SUB0ECS_BENCH_RESTRICT __restrict__
#endif

    namespace tuned
    {
        // The shared kernels' constants, folded exactly as the compiler folds them there.
        inline constexpr float kGravityStep = 9.8f * kDeltaTime;
        inline constexpr float kAngleStep = 0.1f * kDeltaTime;
        inline constexpr float kHealthStep = 0.01f * kDeltaTime;
        inline constexpr float kScaleStep = 1.0f + 0.001f * kDeltaTime;

        inline void iter1(float* SUB0ECS_BENCH_RESTRICT x, std::size_t n)
        {
#if defined(__AVX2__)
            const __m256 one = _mm256_set1_ps(1.0f);
            for (std::size_t i = 0; i < n; i += 8) _mm256_store_ps(x + i, _mm256_add_ps(_mm256_load_ps(x + i), one));
#else
            for (std::size_t i = 0; i < n; ++i) x[i] += 1.0f;
#endif
        }

        /** kernel::updatePosition: the wrap-around tests as selects, in the same order. */
        inline void update2(float* SUB0ECS_BENCH_RESTRICT x, float* SUB0ECS_BENCH_RESTRICT y, float* SUB0ECS_BENCH_RESTRICT dx, float* SUB0ECS_BENCH_RESTRICT dy, std::size_t n)
        {
#if defined(__AVX2__)
            const __m256 dt = _mm256_set1_ps(kDeltaTime), gravity = _mm256_set1_ps(kGravityStep), damp = _mm256_set1_ps(0.99f);
            const __m256 zero = _mm256_setzero_ps(), span = _mm256_set1_ps(1000.0f);
            const auto wrap = [&](__m256 v) {
                v = _mm256_blendv_ps(v, _mm256_add_ps(v, span), _mm256_cmp_ps(v, zero, _CMP_LT_OQ));
                return _mm256_blendv_ps(v, _mm256_sub_ps(v, span), _mm256_cmp_ps(v, span, _CMP_GT_OQ));
            };
            for (std::size_t i = 0; i < n; i += 8)
            {
                const __m256 vx = _mm256_load_ps(dx + i), vy = _mm256_load_ps(dy + i);
                _mm256_store_ps(x + i, wrap(_mm256_add_ps(_mm256_load_ps(x + i), _mm256_mul_ps(vx, dt))));
                _mm256_store_ps(y + i, wrap(_mm256_add_ps(_mm256_load_ps(y + i), _mm256_mul_ps(vy, dt))));
                _mm256_store_ps(dx + i, _mm256_mul_ps(vx, damp));
                _mm256_store_ps(dy + i, _mm256_mul_ps(_mm256_add_ps(vy, gravity), damp));
            }
#else
            const auto wrap = [](float v) {
                v = v < 0.0f ? v + 1000.0f : v;
                return v > 1000.0f ? v - 1000.0f : v;
            };
            for (std::size_t i = 0; i < n; ++i)
            {
                const float vx = dx[i], vy = dy[i];
                x[i] = wrap(x[i] + vx * kDeltaTime);
                y[i] = wrap(y[i] + vy * kDeltaTime);
                dx[i] = vx * 0.99f;
                dy[i] = (vy + kGravityStep) * 0.99f;
            }
#endif
        }

        /** kernel::updateRotationHealth. */
        inline void rotationHealth(float* SUB0ECS_BENCH_RESTRICT health, float* SUB0ECS_BENCH_RESTRICT angle, std::size_t n)
        {
#if defined(__AVX2__)
            const __m256 angleStep = _mm256_set1_ps(kAngleStep), healthStep = _mm256_set1_ps(kHealthStep);
            for (std::size_t i = 0; i < n; i += 8)
            {
                _mm256_store_ps(angle + i, _mm256_add_ps(_mm256_load_ps(angle + i), angleStep));
                _mm256_store_ps(health + i, _mm256_sub_ps(_mm256_load_ps(health + i), healthStep));
            }
#else
            for (std::size_t i = 0; i < n; ++i)
            {
                angle[i] += kAngleStep;
                health[i] -= kHealthStep;
            }
#endif
        }

        /** kernel::pulseScale. */
        inline void pulse(float* SUB0ECS_BENCH_RESTRICT scale, float* SUB0ECS_BENCH_RESTRICT r, float* SUB0ECS_BENCH_RESTRICT g, float* SUB0ECS_BENCH_RESTRICT b, std::size_t n)
        {
#if defined(__AVX2__)
            const __m256 step = _mm256_set1_ps(kScaleStep), one = _mm256_set1_ps(1.0f), two = _mm256_set1_ps(2.0f), half = _mm256_set1_ps(0.5f);
            for (std::size_t i = 0; i < n; i += 8)
            {
                __m256 s = _mm256_mul_ps(_mm256_load_ps(scale + i), step);
                s = _mm256_blendv_ps(s, one, _mm256_cmp_ps(s, two, _CMP_GT_OQ));
                const __m256 up = _mm256_sub_ps(s, one), down = _mm256_sub_ps(two, s);
                _mm256_store_ps(scale + i, s);
                _mm256_store_ps(r + i, _mm256_add_ps(half, _mm256_mul_ps(half, up)));
                _mm256_store_ps(g + i, _mm256_add_ps(half, _mm256_mul_ps(half, down)));
                _mm256_store_ps(b + i, _mm256_add_ps(half, _mm256_mul_ps(half, _mm256_mul_ps(up, down))));
            }
#else
            for (std::size_t i = 0; i < n; ++i)
            {
                float s = scale[i] * kScaleStep;
                s = s > 2.0f ? 1.0f : s;
                scale[i] = s;
                r[i] = 0.5f + 0.5f * (s - 1.0f);
                g[i] = 0.5f + 0.5f * (2.0f - s);
                b[i] = 0.5f + 0.5f * ((s - 1.0f) * (2.0f - s));
            }
#endif
        }
    } // namespace tuned

    class Tuned
    {
    public:
        static constexpr const char* kName = "HandTuned";

        Tuned(std::int64_t n, Pattern pattern)
        {
            std::size_t rows[3] = { 0, 0, 0 };
            for (std::int64_t i = 0; i < n; ++i) ++rows[placeOf(static_cast<std::size_t>(i), pattern).shape];
            for (int shape = 0; shape < 3; ++shape)
            {
                Shape& s = shapes_[shape];
                for (Field* f : { &s.x, &s.y, &s.dx, &s.dy }) f->assign(rows[shape], 0.0f);
                if (shape >= 1)
                {
                    s.health.assign(rows[shape], Health{}.value);
                    s.angle.assign(rows[shape], Rotation{}.angle);
                    s.scale.assign(rows[shape], Scale{}.value);
                }
                if (shape == 2)
                {
                    s.r.assign(rows[shape], Color{}.r);
                    s.g.assign(rows[shape], Color{}.g);
                    s.b.assign(rows[shape], Color{}.b);
                }
            }
            Rng rng;
            for (std::int64_t i = 0; i < n; ++i)
            {
                const Place at = placeOf(static_cast<std::size_t>(i), pattern);
                Shape& s = shapes_[at.shape];
                s.x[at.row] = rng.next();
                s.y[at.row] = rng.next();
                s.dx[at.row] = rng.next();
                s.dy[at.row] = rng.next();
            }
        }

        void iter1()
        {
            for (Shape& s : shapes_) tuned::iter1(s.x.data(), s.x.padded());
        }

        void update2()
        {
            for (Shape& s : shapes_) tuned::update2(s.x.data(), s.y.data(), s.dx.data(), s.dy.data(), s.x.padded());
        }

        void frame3()
        {
            update2();
            for (int shape = 1; shape < 3; ++shape) tuned::rotationHealth(shapes_[shape].health.data(), shapes_[shape].angle.data(), shapes_[shape].health.padded());
            Shape& large = shapes_[2];
            tuned::pulse(large.scale.data(), large.r.data(), large.g.data(), large.b.data(), large.scale.padded());
        }

        float randomGet(const std::vector<std::uint32_t>& handles) const
        {
            float sum = 0.0f;
            for (std::uint32_t h : handles) sum += shapes_[h & 3u].dx[h >> 2];
            return sum;
        }

        double checksum(std::int64_t n, Pattern pattern) const
        {
            double sum = 0.0;
            for (std::int64_t i = 0; i < n; ++i)
            {
                const Place at = placeOf(static_cast<std::size_t>(i), pattern);
                const Shape& s = shapes_[at.shape];
                sum += static_cast<double>(s.x[at.row]) * 3.0 + s.y[at.row];
                sum += static_cast<double>(s.dx[at.row]) * 5.0 + s.dy[at.row] * 7.0;
                if (at.shape >= 1)
                {
                    sum += s.health[at.row] * 11.0;
                    sum += s.angle[at.row] * 29.0;
                    sum += s.scale[at.row] * 13.0;
                }
                if (at.shape == 2) sum += s.r[at.row] * 17.0 + s.g[at.row] * 19.0;
            }
            return sum;
        }

    private:
        struct Shape
        {
            Field x, y, dx, dy;
            Field health, angle, scale;   // Medium and Large
            Field r, g, b;                // Large
        };
        Shape shapes_[3];
    };
} // namespace bench::hand
