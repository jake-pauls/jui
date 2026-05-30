#include "tak/v4.h"

#include <math.h>

f32 tak_v4_length(v4f a)
{
  return sqrtf(tak_v4_length_squared(a));
}

f32 tak_v4_LengthSquared(v4f a)
{
  return
      a.x * a.x +
      a.y * a.y +
      a.z * a.z +
      a.w * a.w;
}

f32 tak_v4_Dot(v4f a, v4f b)
{
  return
      a.x * b.x +
      a.y * b.y +
      a.z * b.z +
      a.w * b.w;
}

v4f tak_v4_Cross(v4f a, v4f b)
{
  return tak_v4(
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x,
      0.0f
      );
}

v4f tak_v4_Normalize(v4f a)
{
  return tak_v4_divide(a, tak_v4_length(a));
}

v4f tak_v4_Lerp(v4f start, v4f end, f32 alpha)
{
  return tak_v4_add(
      tak_v4_multiply(start, 1.0f - alpha),
      tak_v4_multiply(end, alpha)
      );
}