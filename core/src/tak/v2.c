#include "tak/v2.h"

#include <math.h>

f32 tak_v2_length(const v2f A)
{
  const f32 LengthSquared = tak_v2_length_squared(A);
  return sqrtf(LengthSquared);
}

f32 tak_v2_length_squared(const v2f A)
{
  return A.X * A.X + A.Y * A.Y;
}

f32 tak_v2_dot(const v2f A, const v2f B)
{
  return A.X * B.X + A.Y * B.Y;
}

v2f tak_v2_cross(const v2f A, const v2f B)
{
  // In 2D, cross product is scalar in Z; returning XY-style pseudo-vector
  return tak_v2(
      A.X * B.Y - A.Y * B.X,
      A.X * B.Y - A.Y * B.X
      );
}

v2f tak_v2_normalize(const v2f A)
{
  return tak_v2_divide(A, tak_v2_length(A));
}

v2f tak_v2_lerp(const v2f Start, const v2f End, const f32 Alpha)
{
  return tak_v2_add(
      tak_v2_multiply(Start, 1.0f - Alpha),
      tak_v2_multiply(End, Alpha)
      );
}