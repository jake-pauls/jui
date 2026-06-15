#include "jui/core_memory.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void arena_alloc(arena* Arena, const u64 Size)
{
  // TODO(jake): Likely the base functions for this arena could be abstracted at an OS-level (e.g., `mmap` or `VirtualAlloc`).
  Arena->Base = malloc(Size);
  Arena->Capacity = Size;
}

void arena_free(arena* Arena)
{
  free(Arena->Base);
  Arena->Base = NULL;
}

void* arena_push(arena* Arena, const u64 Size)
{
  assert(Arena->Position + Size <= Arena->Capacity);

  // Retrieve the current position of the marker prior to incrementing it.
  void* Memory = Arena->Base + Arena->Position;
  Arena->Position += Size;

  return Memory;
}

void* arena_push_zero(arena* Arena, u64 Size)
{
  void* Memory = arena_push(Arena, Size);
  memset(Memory, 0, Size);
  return Memory;
}

void arena_pop(arena* Arena, const u64 Size)
{
  Arena->Position -= Size;
}

void arena_clear(arena* Arena)
{
  Arena->Position = 0;
}
