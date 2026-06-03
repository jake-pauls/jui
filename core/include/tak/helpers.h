#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;
typedef int i32;

#define TAK_PI 3.1415926535897932f

/// \brief Converts degrees to radians
/// \param Degrees Degrees to convert
/// \returns The provided float in degrees as radians
static inline f32 tak_deg_to_rad(f32 Degrees)
{
  return Degrees * (TAK_PI / 180.0f);
}

/// \brief Converts radians to degrees
/// \param Radians Radians to convert
/// \returns The provided float in radians as degrees
static inline f32 tak_rad_to_deg(f32 Radians)
{
  return Radians * (180.0f / TAK_PI);
}

/// \brief Clamps an integer value between a minimum and a maximum
/// \param A The input integer
/// \param Min The minimum-bound for the integer value
/// \param Max The maximum-bound for the integer value
/// \returns The provided integer clamped between the provided min and max
i32 tak_clamp_i32(i32 A, i32 Min, i32 Max);

/// \brief Clamps a float value between a minimum and a maximum
/// \param A The input float
/// \param Min The minimum-bound for the float value
/// \param Max The maximum-bound for the float value
/// \returns The provided float clamped between the provided min and max
f32 tak_clamp_f32(f32 A, f32 Min, f32 Max);

/// \brief Returns an integer value on the linear function of x and y at a provided alpha
/// \param X,Y The two coordinates comprising a linear relationship
/// \param Alpha The value on the linear relationship to return
/// \returns Value on the linear function of x and y at the provided alpha
i32 tak_lerp_i32(i32 X, i32 Y, i32 Alpha);

/// \brief Returns a float value on the linear function of x and y at a provided alpha
/// \param X,Y The two coordinates comprising a linear relationship
/// \param Alpha The value on the linear relationship to return
/// \returns Value on the linear function of x and y at the provided alpha
f32 tak_lerp_f32(f32 X, f32 Y, f32 Alpha);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

namespace tak
{
  inline f32 DegToRad(f32 Degrees) { return tak_deg_to_rad(Degrees); }
  inline f32 RadToDeg(f32 Radians) { return tak_rad_to_deg(Radians); }

  inline i32 Clamp(i32 A, i32 Min, i32 Max) { return tak_clamp_i32(A, Min, Max); }
  inline f32 Clamp(f32 A, f32 Min, f32 Max) { return tak_clamp_f32(A, Min, Max); }

  inline i32 Lerp(i32 X, i32 Y, i32 Alpha) { return tak_lerp_i32(X, Y, Alpha); }
  inline f32 Lerp(f32 X, f32 Y, f32 Alpha) { return tak_lerp_f32(X, Y, Alpha); }
}

#endif