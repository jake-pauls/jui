#include "tak/helpers.h"

i32 tak_clamp_i32(i32 A, i32 Min, i32 Max)
{
  const i32 B = A < Min ? Min : A;
  return B > Max ? Max : B;
}

f32 tak_clamp_f32(f32 A, f32 Min, f32 Max)
{
  const f32 B = A < Min ? Min : A;
  return B > Max ? Max : B;
}

i32 tak_lerp_i32(i32 X, i32 Y, i32 Alpha)
{
  return (1 - Alpha) * X + Alpha * Y;
}

f32 tak_lerp_f32(f32 X, f32 Y, f32 Alpha)
{
  return (1.0f - Alpha) * X + Alpha * Y;
}