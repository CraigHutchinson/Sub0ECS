#pragma once
namespace bench::rows
{
/** Complete-tick work shape; each has the same deterministic scalar oracle. */
enum class Scenario
{
    Streaming,
    FragmentedChurn,
    StagedNeighbors
};
}
