#include "jui/hash.h"

u64 hash_djb2(const uchar* Str)
{
  u64 Hash = 5381;
  i32 C;

  while ((C = *Str++))
  {
    Hash = (Hash << 5) + Hash + C; /* Hash * 33 + C */
  }

  return Hash;
}
