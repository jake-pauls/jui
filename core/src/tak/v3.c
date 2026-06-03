#include "tak/v3.h"

#include <math.h>

f32 tak_v3_length(const v3f A)
{
  const f32 LengthSquared = tak_v3_length_squared(A);
  return sqrtf(LengthSquared);
}

f32 tak_v3_length_squared(const v3f A)
{
  return
      A.X * A.X +
      A.Y * A.Y +
      A.Z * A.Z;
}

f32 tak_v3_dot(const v3f A, const v3f B)
{
  return
      A.X * B.X +
      A.Y * B.Y +
      A.Z * B.Z;
}

v3f tak_v3_cross(const v3f A, const v3f B)
{
  return tak_v3(
      A.Y * B.Z - A.Z * B.Y,
      A.Z * B.X - A.X * B.Z,
      A.X * B.Y - A.Y * B.X
      );
}

v3f tak_v3_normalize(const v3f A)
{
  return tak_v3_divide(A, tak_v3_length(A));
}

v3f take_v3_lerp(v3f Start, v3f End, f32 Alpha)
{
  return tak_v3_add(
      tak_v3_multiply(Start, 1.0f - Alpha),
      tak_v3_multiply(End, Alpha)
      );
}