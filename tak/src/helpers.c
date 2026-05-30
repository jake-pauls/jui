#include "tak/helpers.h"

i32 tak_clamp_i32(i32 a, i32 min, i32 max)
{
  const i32 b = a < min ? min : a;
  return b > max ? max : b;
}

f32 tak_clamp_f32(f32 a, f32 min, f32 max)
{
  const f32 b = a < min ? min : a;
  return b > max ? max : b;
}

i32 tak_lerp_i32(i32 x, i32 y, i32 alpha)
{
  return (1 - alpha) * x + alpha * y;
}

f32 tak_lerp_f32(f32 x, f32 y, f32 alpha)
{
  return (1.0f - alpha) * x + alpha * y;
}