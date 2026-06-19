#pragma once

#include "jui/types.h"

#include <math.h>
#include <stdbool.h>

/* Core Structures */

/// \brief Represents the unique identifier for a widget in jui.
typedef u64 jui_id;

/// \brief Represents a two-float vector. Used to represent arbitrary vectors.
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

/// \brief Represents a three-float vector. Used to represent arbitrary vectors.
typedef union jui_v3 jui_v3;
union jui_v3
{
  struct
  {
    f32 X;
    f32 Y;
    f32 Z;
  };
  f32 V[3];
};

/// \brief Represents a rectangle using floating point values.
typedef struct jui_rect jui_rect;
struct jui_rect
{
  f32 X, Y, W, H;
};

/// \brief Represents a color of four eight-bit unsigned integers.
typedef struct jui_color jui_color;
struct jui_color
{
  u8 R, G, B, A;
};

/* Math */

#define JUI_EPSILON 0.00001f

static bool almost_equal(const f32 A, const f32 B, const f32 Epsilon)
{
  return fabs(B - A) < Epsilon;
}

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
/// \remarks <a href="http://www.csegyorku.ca/~oz/hash.html">Reference</a>
u64 hash_djb2(const uchar* Str);
