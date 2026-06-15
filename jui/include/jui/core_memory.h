#pragma once

#include "jui/types.h"

typedef struct arena arena;
struct arena
{
  void* Base;
  u64 Capacity;
  u64 Position;
};

/// \brief Allocates an arena with a size in bytes.
/// \param Arena Pointer to the arena to reserve memory for.
/// \param Size The size in bytes to reserve for the arena.
void arena_alloc(arena* Arena, u64 Size);

/// \brief Frees all reserved memory in the arena.
/// \param Arena Pointer to the arena to free all the reserved memory of.
void arena_free(arena* Arena);

/// \brief Pushes bytes onto the arena, reserving a specified size in bytes from its base memory.
/// \param Arena Pointer to the arena to push to.
/// \param Size The size in bytes to push onto the arena.
void* arena_push(arena* Arena, u64 Size);

/// \brief Pushes bytes onto the arena, zero-initializing all bytes in the segment.
/// \param Arena Pointer to the arena to push to.
/// \param Size The size in bytes to push onto the arena.
void* arena_push_zero(arena* Arena, u64 Size);

/// \brief Pops a specified size in bytes off of the arena.
/// \param Arena Pointer to the arena to pop.
/// \param Size The size in bytes to pop off of the arena.
void arena_pop(arena* Arena, u64 Size);

/// \brief Clears all bytes off of the arena, resetting it to the base address.
/// \param Arena Pointer to the arena to pop all the bytes of.
void arena_clear(arena* Arena);

#define PUSH_ARRAY(Arena, Type, Count) ((Type*)arena_push((Arena), sizeof(Type) * Count))
#define PUSH_ARRAY_ZERO(Arena, Type, Count) ((Type*)arena_push_zero((Arena), sizeof(Type) * Count))
#define PUSH_STRUCT(Arena, Type) ((Type*)arena_push((Arena), sizeof(Type)))
#define PUSH_STRUCT_ZERO(Arena, Type) ((Type*)arena_push_zero((Arena), sizeof(Type)))