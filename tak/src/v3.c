#include "tak/v3.h"

#include <math.h>

f32 tak_v3_length(V3F A)
{
  return sqrt(tak_v3_length_squared(A));
}

f32 tak_v3_length_squared(V3F A)
{
  return
      A.X * A.X +
      A.Y * A.Y +
      A.Z * A.Z;
}

f32 tak_v3_dot(V3F A, V3F B)
{
  return
      A.X * B.X +
      A.Y * B.Y +
      A.Z * B.Z;
}

V3F tak_v3_cross(V3F A, V3F B)
{
  return tak_v3(
      A.Y * B.Z - A.Z * B.Y,
      A.Z * B.X - A.X * B.Z,
      A.X * B.Y - A.Y * B.X
      );
}

V3F tak_v3_normalize(V3F A)
{
  return tak_v3_divide(A, tak_v3_length(A));
}

V3F take_v3_lerp(V3F Start, V3F End, f32 Alpha)
{
  return tak_v3_add(
      tak_v3_multiply(Start, 1.0f - Alpha),
      tak_v3_multiply(End, Alpha)
      );
}