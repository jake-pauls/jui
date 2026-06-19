#pragma once

#include "jui/types.h"

/* Core Structures */

/// \brief Represents a two-float vector. Used to represent arbitrary position vectors.
//         Use another struct to represent something more meaningful, for example,
//         for rects refer to \see jui_rect.
typedef union jui_v2 jui_v2;
union jui_v2
{
  struct
  {
    f32 X;
    f32 Y;
  };
  f32 V[2];
};

/* Hashing */

/// \brief djb2 hash function for strings. The hash function itself has
///        clever properties that make it well-suited for hashing strings:
///
/// 1. Use of <c>unsigned char</c> ensures that incoming bytes are guaranteed to
///    be in the range 0 <= C <= 255.
///
/// 2. The hash uses 33 as the base (<c>(Hash << 5) + Hash</c>) since it's odd,
///    close to a power of two, and sufficiently mixes the input (as opposed to
///    using a power of two directly).
///
/// \param Str Input string to hash.
/// \remark <a href="http://www.csegyorku.ca/~oz/hash.html">Reference</a>
u64 hash_djb2(const uchar* Str);
