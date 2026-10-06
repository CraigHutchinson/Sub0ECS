#pragma once
/** Skirmish testbed — components and shared unit data.
 *
 * All components are small trivially-copyable PODs (a requirement of the
 * archetype / query-partition designs, and realistic for an ECS).
 */

#include <cstdint>

namespace skirmish
{
    // ---- identity & spatial ------------------------------------------------
    struct Id { std::uint32_t value = 0; };          ///< stable serial id (0 = none); canonical order key
    struct Position { float x = 0.0f; float y = 0.0f; };
    struct Velocity { float x = 0.0f; float y = 0.0f; };
    struct Team { std::int32_t value = 0; };

    // ---- units ----------------------------------------------------------------
    struct UnitType { std::int32_t kind = 0; };      ///< index into kUnitStats (shared data, flyweight)
    struct Health { float hp = 0.0f; float maxHp = 0.0f; std::uint32_t lastHitBy = 0; };
    struct Weapon { float cooldown = 0.0f; std::uint32_t shots = 0; };
    struct MoveOrder { float tx = 0.0f; float ty = 0.0f; };          ///< churn: orders come and go
    struct Target { std::uint32_t id = 0; };                         ///< churn: acquired / lost
    struct Worker { std::uint32_t homeId = 0; std::uint32_t mineId = 0; std::int32_t team = 0; };
    struct Carrying { std::int32_t amount = 0; };                    ///< churn, never queried (side storage)
    struct Veteran { std::int32_t kills = 0; };                      ///< rare, stable

    // ---- status effects (rare, volatile) ------------------------------------
    struct Stunned { std::int32_t ticks = 0; };
    struct Burning { std::int32_t ticks = 0; };
    struct Selected { std::int32_t group = 0; };                      ///< ~1% "UI" selection

    // ---- projectiles (high spawn/destroy churn) ------------------------------
    struct Projectile
    {
        std::uint32_t targetId = 0;
        std::uint32_t ownerId = 0;
        float damage = 0.0f;
        std::int32_t ttl = 0;
        std::int32_t effect = 0;   ///< 0 none, 1 burn, 2 stun
    };

    // ---- structures ----------------------------------------------------------
    struct Building { std::int32_t kind = 0; };   ///< 0 HQ, 1 barracks
    struct Producer { float progress = 0.0f; std::int32_t counter = 0; };
    struct ResourceNode { std::int32_t amount = 0; };

    // ---- shared unit data (one authority per kind) --------------------------
    enum Kind : std::int32_t { kWorker = 0, kSoldier = 1, kArcher = 2, kKnight = 3, kKinds = 4 };
    enum Effect : std::int32_t { kNoEffect = 0, kBurn = 1, kStun = 2 };

    struct UnitStats
    {
        float maxHp, speed, range, damage, reload, projectileSpeed, regen;
        std::int32_t effect, effectEvery, cost;
    };

    inline constexpr UnitStats kUnitStats[kKinds] = {
        //  hp   speed range  dmg  reload projSpd regen effect    every cost
        { 40.f, 60.f, 0.f, 0.f, 0.f, 0.f, 0.5f, kNoEffect, 0, 20 },       // worker
        { 100.f, 45.f, 18.f, 10.f, 0.8f, 400.f, 0.5f, kNoEffect, 0, 40 }, // soldier
        { 60.f, 40.f, 90.f, 7.f, 1.2f, 220.f, 0.5f, kBurn, 6, 50 },       // archer
        { 160.f, 70.f, 20.f, 14.f, 1.0f, 400.f, 0.5f, kStun, 5, 80 },     // knight
    };

    inline constexpr float kDt = 1.0f / 30.0f;       ///< fixed 30 Hz lockstep tick
    inline constexpr float kAcquireExtra = 60.0f;     ///< acquire radius = range + this
    inline constexpr float kSeparation = 10.0f;       ///< unit personal-space radius
    inline constexpr float kCellSize = 32.0f;         ///< spatial grid cell size

} // namespace skirmish
