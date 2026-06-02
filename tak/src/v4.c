#include "tak/v4.h"

#include <math.h>

f32 tak_v4_length(V4F A)
{
  return sqrtf(tak_v4_length_squared(A));
}

f32 tak_v4_LengthSquared(V4F a)
{
  return
      a.X * a.X +
      a.Y * a.Y +
      a.Z * a.Z +
      a.W * a.W;
}

f32 tak_v4_Dot(V4F a, V4F b)
{
  return
      a.X * b.X +
      a.Y * b.Y +
      a.Z * b.Z +
      a.W * b.W;
}

V4F tak_v4_Cross(V4F a, V4F b)
{
  return tak_v4(
      a.Y * b.Z - a.Z * b.Y,
      a.Z * b.X - a.X * b.Z,
      a.X * b.Y - a.Y * b.X,
      0.0f
      );
}

V4F tak_v4_Normalize(V4F a)
{
  return tak_v4_divide(a, tak_v4_length(a));
}

V4F tak_v4_Lerp(V4F start, V4F end, f32 alpha)
{
  return tak_v4_add(
      tak_v4_multiply(start, 1.0f - alpha),
      tak_v4_multiply(end, alpha)
      );
}