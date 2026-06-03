#include "tak/v4.h"

#include <math.h>

f32 tak_v4_length(const v4f A)
{
  const f32 LengthSquared = tak_v4_length_squared(A);
  return sqrtf(LengthSquared);
}

f32 tak_v4_LengthSquared(const v4f A)
{
  return
      A.X * A.X +
      A.Y * A.Y +
      A.Z * A.Z +
      A.W * A.W;
}

f32 tak_v4_Dot(const v4f A, const v4f B)
{
  return
      A.X * B.X +
      A.Y * B.Y +
      A.Z * B.Z +
      A.W * B.W;
}

v4f tak_v4_Cross(const v4f A, const v4f B)
{
  return tak_v4(
      A.Y * B.Z - A.Z * B.Y,
      A.Z * B.X - A.X * B.Z,
      A.X * B.Y - A.Y * B.X,
      0.0f
      );
}

v4f tak_v4_Normalize(const v4f A)
{
  return tak_v4_divide(A, tak_v4_length(A));
}

v4f tak_v4_Lerp(const v4f Start, const v4f End, const f32 Alpha)
{
  return tak_v4_add(
      tak_v4_multiply(Start, 1.0f - Alpha),
      tak_v4_multiply(End, Alpha)
      );
}