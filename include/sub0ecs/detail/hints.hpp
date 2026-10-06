#pragma once
/** Optimiser hints that need a different spelling per compiler.
 *
 * SUB0ECS_FLATTEN_CALLS prefixes one statement: every call in it is inlined,
 * and so are the calls those make. It goes on the per-row call of an iteration
 * loop, where the body is a user callable the library cannot annotate.
 *
 * MSVC only. Its inliner stops one level short there: the callable is inlined
 * but a function it calls stays a call per row. GCC and Clang inline that
 * chain unaided, and have no statement-level equivalent. */

#if defined(_MSC_VER) && !defined(__clang__) && _MSC_VER >= 1937
#    define SUB0ECS_FLATTEN_CALLS [[msvc::flatten]]
#else
#    define SUB0ECS_FLATTEN_CALLS
#endif
