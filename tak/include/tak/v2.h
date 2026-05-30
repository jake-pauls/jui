#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;

/// \brief Two-component float vector
typedef union v2f
{
  struct
  {
    f32 x;
    f32 y;
  };
  f32 v[2];
} v2f;

/// Constructor
static inline v2f tak_v2(f32 x, f32 y)
{
  v2f result;
  result.x = x;
  result.y = y;
  return result;
}

/// Unary minus
static inline v2f tak_v2_neg(v2f a)
{
  return tak_v2(-a.x, -a.y);
}

/// Add
static inline v2f tak_v2_add(v2f a, v2f b)
{
  return tak_v2(a.x + b.x, a.y + b.y);
}

/// Subtract
static inline v2f tak_v2_subtract(v2f a, v2f b)
{
  return tak_v2(a.x - b.x, a.y - b.y);
}

/// Multiply scalar
static inline v2f tak_v2_multiply(v2f a, f32 s)
{
  return tak_v2(a.x * s, a.y * s);
}

/// Divide scalar
static inline v2f tak_v2_divide(v2f a, f32 s)
{
  return tak_v2(a.x / s, a.y / s);
}

/// \brief returns the length of the provided vector
/// \param a the vector to get the length of
/// \returns the length of the vector
f32 tak_v2_length(v2f a);

/// \brief Returns the squared length of the provided vector
/// \param a The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v2_length_squared(v2f a);

/// \brief The dot product of two vectors
/// \param a,b The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v2_dot(v2f a, v2f b);

/// \brief The cross product of two vectors
/// \param a,b The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
v2f tak_v2_cross(v2f a, v2f b);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param a The vector to normalize
/// \returns The normalized version of the vector a
v2f tak_v2_normalize(v2f a);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param start,end The two vectors in-space comprising a linear relationship
/// \param alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
v2f tak_v2_lerp(v2f start, v2f end, f32 alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using::v2f;

  inline v2f operator-(v2f a) { return tak_v2_neg(a); }
  inline v2f operator+(v2f a, v2f b) { return tak_v2_add(a, b); }
  inline v2f operator-(v2f a, v2f b) { return tak_v2_subtract(a, b); }
  inline v2f operator*(v2f a, f32 s) { return tak_v2_multiply(a, s); }
  inline v2f operator*(f32 s, v2f a) { return tak_v2_multiply(a, s); }
  inline v2f operator/(v2f a, f32 s) { return tak_v2_divide(a, s); }
  inline f32 Length(v2f a) { return tak_v2_length(a); }
  inline f32 LengthSquared(v2f a) { return tak_v2_length_squared(a); }
  inline f32 Dot(v2f a, v2f b) { return tak_v2_dot(a, b); }
  inline v2f Cross(v2f a, v2f b) { return tak_v2_cross(a, b); }
  inline v2f Normalize(v2f a) { return tak_v2_normalize(a); }
  inline v2f Lerp(v2f a, v2f b, f32 t) { return tak_v2_lerp(a, b, t); }
}

#endif