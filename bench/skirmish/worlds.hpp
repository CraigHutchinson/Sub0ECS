#pragma once
/** Storage designs instantiated for the Skirmish component set.
 *
 * Query-partition designs need the game's system queries declared up front
 * (that is the point of the design): every each<...> signature used by sim.hpp.
 */

#include "../designs/archetype.hpp"
#include "../designs/query_partition.hpp"
#include "../designs/sorted_soa.hpp"
#include "../designs/sparse_set.hpp"
#include "../designs/static_bitmask.hpp"
#include "components.hpp"

namespace skirmish
{
    using SkirmishQueries = std::tuple<
        Query<Id, Team, UnitType>,                            // population, selection pick
        Query<Id, Building, Producer, Team, Position>,        // production
        Query<Id, Position, Team, Health>,                    // grid build
        Query<Id, Selected, Position>,                        // selection reduction
        Query<Id, Position, Team, Weapon, UnitType>,          // acquire, commander
        Query<Id, Worker, Position>,                          // workers
        Query<MoveOrder, Position, Velocity, UnitType>,       // Seek        (fusable)
        Query<Id, Position, Velocity, Team>,                  // Separation  (fusable, reads grid snapshot)
        Query<Stunned, Velocity>,                             // StunFreeze  (fusable)
        Query<Position, Velocity>,                            // Integrate, Bounds (fusable)
        Query<Velocity, UnitType>,                            // Friction    (fusable)
        Query<Id, MoveOrder, Position>,                       // arrive
        Query<Id, Position, Weapon, Target, Team, UnitType>,  // combat
        Query<Id, Projectile, Position, Velocity>,            // projectiles
        Query<Id, Stunned>,                                   // stun countdown
        Query<Id, Burning, Health>,                           // burning
        Query<Id, Health, Team>,                              // death
        Query<Health, UnitType>>;                             // regen

    using SparseSetWorld = bench::sparse::World;
    using ArchetypeWorld = bench::archetype::World;
    using SortedWorld = bench::sorted::World;
    using QueryPartWorld = sub0ecs::store::BasicWorld<false, SkirmishQueries>;
    using QPartHintedWorld = sub0ecs::store::BasicWorld<true, SkirmishQueries, sub0ecs::store::Volatile<Carrying>>;

    template <std::size_t Capacity>
    using StaticWorld = bench::fixed::BasicWorld<Capacity, Id, Position, Velocity, Team, UnitType, Health, Weapon,
                                                 MoveOrder, Target, Worker, Carrying, Veteran, Stunned, Burning,
                                                 Selected, Projectile, Building, Producer, ResourceNode>;

} // namespace skirmish
