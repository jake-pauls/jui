#include "jui/core_memory.h"

#include <stdlib.h>

void arena_alloc(arena* Arena, const u64 Size)
{
  // TODO(jake): Likely the base functions for this arena could be abstracted at an OS-level (e.g., `mmap` or `VirtualAlloc`).
  Arena->Base = malloc(Size);
}

void arena_free(arena* Arena)
{
  free(Arena->Base);
  Arena->Base = NULL;
}

void* arena_push(arena* Arena, const u64 Size)
{
  // Retrieve the current position of the marker prior to incrementing it.
  void* Memory = Arena->Base + Arena->Position;
  Arena->Position += Size;
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
