#include "tak/v2.h"

#include <math.h>

f32 tak_v2_length(V2F A)
{
  return sqrtf(tak_v2_length_squared(A));
}

f32 tak_v2_length_squared(V2F A)
{
  return A.X * A.X + A.Y * A.Y;
}

f32 tak_v2_dot(V2F A, V2F B)
{
  return A.X * B.X + A.Y * B.Y;
}

V2F tak_v2_cross(V2F A, V2F B)
{
  // In 2D, cross product is scalar in Z; returning XY-style pseudo-vector
  return tak_v2(
      A.X * B.Y - A.Y * B.X,
      A.X * B.Y - A.Y * B.X
      );
}

V2F tak_v2_normalize(V2F A)
{
  return tak_v2_divide(A, tak_v2_length(A));
}

V2F tak_v2_lerp(V2F Start, V2F End, f32 Alpha)
{
  return tak_v2_add(
      tak_v2_multiply(Start, 1.0f - Alpha),
      tak_v2_multiply(End, Alpha)
      );
}