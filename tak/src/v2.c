#include "tak/v2.h"

#include <math.h>

f32 tak_v2_length(v2f a)
{
  return sqrtf(tak_v2_length_squared(a));
}

f32 tak_v2_length_squared(v2f a)
{
  return a.x * a.x + a.y * a.y;
}

f32 tak_v2_dot(v2f a, v2f b)
{
  return a.x * b.x + a.y * b.y;
}

v2f tak_v2_cross(v2f a, v2f b)
{
  // In 2D, cross product is scalar in Z; returning XY-style pseudo-vector
  return tak_v2(
      a.x * b.y - a.y * b.x,
      a.x * b.y - a.y * b.x
      );
}

v2f tak_v2_normalize(v2f a)
{
  return tak_v2_divide(a, tak_v2_length(a));
}

v2f tak_v2_lerp(v2f start, v2f end, f32 alpha)
{
  return tak_v2_add(
      tak_v2_multiply(start, 1.0f - alpha),
      tak_v2_multiply(end, alpha)
      );
}