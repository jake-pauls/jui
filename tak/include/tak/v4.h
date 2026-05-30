#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;

/// \brief Four-component float vector
typedef union v4f
{
  struct
  {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
  };
  f32 v[4];
} v4f;

/// \brief Creates a four-component vector
static inline v4f tak_v4(f32 x, f32 y, f32 z, f32 w)
{
  v4f result;

  result.x = x;
  result.y = y;
  result.z = z;
  result.w = w;

  return result;
}

/// \brief Negates the provided vector
static inline v4f tak_v4_neg(v4f a)
{
  return tak_v4(
      -a.x,
      -a.y,
      -a.z,
      -a.w
      );
}

/// \brief Adds two vectors
static inline v4f tak_v4_add(v4f a, v4f b)
{
  return tak_v4(
      a.x + b.x,
      a.y + b.y,
      a.z + b.z,
      a.w + b.w
      );
}

/// \brief Subtracts two vectors
static inline v4f tak_v4_subtract(v4f a, v4f b)
{
  return tak_v4(
      a.x - b.x,
      a.y - b.y,
      a.z - b.z,
      a.w - b.w
      );
}

/// \brief Multiplies a vector by a scalar
static inline v4f tak_v4_multiply(v4f a, f32 scalar)
{
  return tak_v4(
      a.x * scalar,
      a.y * scalar,
      a.z * scalar,
      a.w * scalar
      );
}

/// \brief Divides a vector by a scalar
static inline v4f tak_v4_divide(v4f a, f32 scalar)
{
  return tak_v4(
      a.x / scalar,
      a.y / scalar,
      a.z / scalar,
      a.w / scalar
      );
}

/// \brief returns the length of the provided vector
/// \param a the vector to get the length of
/// \returns the length of the vector
f32 tak_v4_length(v4f a);

/// \brief Returns the squared length of the provided vector
/// \param a The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v4_length_squared(v4f a);

/// \brief The dot product of two vectors
/// \param a,b The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v4_dot(v4f a, v4f b);

/// \brief The cross product of two vectors
/// \param a,b The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
v4f tak_v4_cross(v4f a, v4f b);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param a The vector to normalize
/// \returns The normalized version of the vector a
v4f tak_v4_normalize(v4f a);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param start,end The two vectors in-space comprising a linear relationship
/// \param alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
v4f tak_v4_lerp(v4f start, v4f end, f32 alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using::v4f;

  inline v4f operator-(v4f a) { return tak_v4_neg(a); }
  inline v4f operator+(v4f a, v4f b) { return tak_v4_add(a, b); }
  inline v4f operator-(v4f a, v4f b) { return tak_v4_subtract(a, b); }
  inline v4f operator*(v4f a, f32 scalar) { return tak_v4_multiply(a, scalar); }
  inline v4f operator*(f32 scalar, v4f a) { return tak_v4_multiply(a, scalar); }
  inline v4f operator/(v4f a, f32 scalar) { return tak_v4_divide(a, scalar); }
  inline f32 Length(v4f a) { return tak_v4_length(a); }
  inline f32 LengthSquared(v4f a) { return tak_v4_length_squared(a); }
  inline f32 Dot(v4f a, v4f b) { return tak_v4_dot(a, b); }
  inline v4f Cross(v4f a, v4f b) { return tak_v4_cross(a, b); }
  inline v4f Normalize(v4f a) { return tak_v4_normalize(a); }
  inline v4f Lerp(v4f start, v4f end, f32 alpha) { return tak_v4_lerp(start, end, alpha); }
}

#endif