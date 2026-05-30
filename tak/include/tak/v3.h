#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;

/// \brief Three-component float vector
typedef union v3f
{
  struct
  {
    f32 x;
    f32 y;
    f32 z;
  };
  f32 v[3];
} v3f;

/// Constructors/helpers
static inline v3f tak_v3(f32 x, f32 y, f32 z)
{
  v3f result;
  result.x = x;
  result.y = y;
  result.z = z;
  return result;
}

/// Unary Negation
static inline v3f tak_v3_neg(v3f a)
{
  return tak_v3(-a.x, -a.y, -a.z);
}

/// Addition
static inline v3f tak_v3_add(v3f a, v3f b)
{
  return tak_v3(
      a.x + b.x,
      a.y + b.y,
      a.z + b.z
      );
}

/// Subtraction
static inline v3f tak_v3_subtract(v3f a, v3f b)
{
  return tak_v3(
      a.x - b.x,
      a.y - b.y,
      a.z - b.z
      );
}

/// Scalar multiply
static inline v3f tak_v3_multiply(v3f a, f32 s)
{
  return tak_v3(
      a.x * s,
      a.y * s,
      a.z * s
      );
}

/// Scalar divide
static inline v3f tak_v3_divide(v3f a, f32 s)
{
  return tak_v3(
      a.x / s,
      a.y / s,
      a.z / s
      );
}

/// \brief Returns the length of the provided vector
/// \param a The vector to get the length of
/// \returns The length of the vector
f32 tak_v3_length(v3f a);

/// \brief Returns the squared length of the provided vector
/// \param a The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v3_length_squared(v3f a);

/// \brief The dot product of two vectors
/// \param a,b The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v3_dot(v3f a, v3f b);

/// \brief The cross product of two vectors
/// \param a,b The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
v3f tak_v3_cross(v3f a, v3f b);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param a The vector to normalize
/// \returns The normalized version of the vector a
v3f tak_v3_normalize(v3f a);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param start,end The two vectors in-space comprising a linear relationship
/// \param alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
v3f take_v3_lerp(v3f start, v3f end, f32 alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using::v3f;

  inline v3f operator-(v3f a) { return tak_v3_neg(a); }
  inline v3f operator+(v3f a, v3f b) { return tak_v3_add(a, b); }
  inline v3f operator-(v3f a, v3f b) { return tak_v3_subtract(a, b); }
  inline v3f operator*(v3f a, f32 s) { return tak_v3_multiply(a, s); }
  inline v3f operator*(f32 s, v3f a) { return tak_v3_multiply(a, s); }
  inline v3f operator/(v3f a, f32 s) { return tak_v3_divide(a, s); }
  inline f32 Length(v3f a) { return tak_v3_length(a); }
  inline f32 LengthSquared(v3f a) { return tak_v3_length_squared(a); }
  inline f32 Dot(v3f a, v3f b) { return tak_v3_dot(a, b); }
  inline v3f Cross(v3f a, v3f b) { return tak_v3_cross(a, b); }
  inline v3f Normalize(v3f a) { return tak_v3_normalize(a); }
  inline v3f Lerp(v3f start, v3f end, f32 alpha) { return take_v3_lerp(start, end, alpha); }
}

#endif