#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;
typedef int i32;

#define TAK_PI 3.1415926535897932f

/// \brief Converts degrees to radians
/// \param deg Degrees to convert
/// \returns The provided float in degrees as radians
static inline f32 tak_deg_to_rad(f32 deg)
{
  return deg * (TAK_PI / 180.0f);
}

/// \brief Converts radians to degrees
/// \param rad Radians to convert
/// \returns The provided float in radians as degrees
static inline f32 tak_rad_to_deg(f32 rad)
{
  return rad * (180.0f / TAK_PI);
}

/// \brief Clamps an integer value between a minimum and a maximum
/// \param a The input integer
/// \param min The minimum-bound for the integer value
/// \param max The maximum-bound for the integer value
/// \returns The provided integer clamped between the provided min and max
i32 tak_clamp_i32(i32 a, i32 min, i32 max);

/// \brief Clamps a float value between a minimum and a maximum
/// \param a The input float
/// \param min The minimum-bound for the float value
/// \param max The maximum-bound for the float value
/// \returns The provided float clamped between the provided min and max
f32 tak_clamp_f32(f32 a, f32 min, f32 max);

/// \brief Returns an integer value on the linear function of x and y at a provided alpha
/// \param x,y The two coordinates comprising a linear relationship
/// \param alpha The value on the linear relationship to return
/// \returns Value on the linear function of x and y at the provided alpha
i32 tak_lerp_i32(i32 x, i32 y, i32 alpha);

/// \brief Returns a float value on the linear function of x and y at a provided alpha
/// \param x,y The two coordinates comprising a linear relationship
/// \param alpha The value on the linear relationship to return
/// \returns Value on the linear function of x and y at the provided alpha
f32 tak_lerp_f32(f32 x, f32 y, f32 alpha);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

namespace tak
{
  inline f32 DegToRad(f32 deg) { return tak_deg_to_rad(deg); }
  inline f32 RadToDeg(f32 rad) { return tak_rad_to_deg(rad); }

  inline i32 Clamp(i32 a, i32 min, i32 max) { return tak_clamp_i32(a, min, max); }
  inline f32 Clamp(f32 a, f32 min, f32 max) { return tak_clamp_f32(a, min, max); }

  inline i32 Lerp(i32 x, i32 y, i32 alpha) { return tak_lerp_i32(x, y, alpha); }
  inline f32 Lerp(f32 x, f32 y, f32 alpha) { return tak_lerp_f32(x, y, alpha); }
}

#endif