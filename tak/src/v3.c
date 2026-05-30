#include "tak/v3.h"

#include <math.h>

f32 tak_v3_length(v3f a)
{
  return sqrt(tak_v3_length_squared(a));
}

f32 tak_v3_length_squared(v3f a)
{
  return
      a.x * a.x +
      a.y * a.y +
      a.z * a.z;
}

f32 tak_v3_dot(v3f a, v3f b)
{
  return
      a.x * b.x +
      a.y * b.y +
      a.z * b.z;
}

v3f tak_v3_cross(v3f a, v3f b)
{
  return tak_v3(
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x
      );
}

v3f tak_v3_normalize(v3f a)
{
  return tak_v3_divide(a, tak_v3_length(a));
}

v3f take_v3_lerp(v3f start, v3f end, f32 alpha)
{
  return tak_v3_add(
      tak_v3_multiply(start, 1.0f - alpha),
      tak_v3_multiply(end, alpha)
      );
}