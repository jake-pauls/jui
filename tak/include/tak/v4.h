#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;

/// \brief Four-component float vector
typedef union V4F
{
  struct
  {
    f32 X;
    f32 Y;
    f32 Z;
    f32 W;
  };
  f32 V[4];
} V4F;

/// \brief Creates a four-component vector
static inline V4F tak_v4(f32 X, f32 Y, f32 Z, f32 W)
{
  V4F Result;
  Result.X = X;
  Result.Y = Y;
  Result.Z = Z;
  Result.W = W;
  return Result;
}

/// \brief Negates the provided vector
static inline V4F tak_v4_neg(V4F A)
{
  return tak_v4(
      -A.X,
      -A.Y,
      -A.Z,
      -A.W
      );
}

/// \brief Adds two vectors
static inline V4F tak_v4_add(V4F A, V4F B)
{
  return tak_v4(
      A.X + B.X,
      A.Y + B.Y,
      A.Z + B.Z,
      A.W + B.W
      );
}

/// \brief Subtracts two vectors
static inline V4F tak_v4_subtract(V4F A, V4F B)
{
  return tak_v4(
      A.X - B.X,
      A.Y - B.Y,
      A.Z - B.Z,
      A.W - B.W
      );
}

/// \brief Multiplies a vector by a scalar
static inline V4F tak_v4_multiply(V4F A, f32 S)
{
  return tak_v4(
      A.X * S,
      A.Y * S,
      A.Z * S,
      A.W * S
      );
}

/// \brief Divides a vector by a scalar
static inline V4F tak_v4_divide(V4F A, f32 S)
{
  return tak_v4(
      A.X / S,
      A.Y / S,
      A.Z / S,
      A.W / S
      );
}

/// \brief returns the length of the provided vector
/// \param A the vector to get the length of
/// \returns the length of the vector
f32 tak_v4_length(V4F A);

/// \brief Returns the squared length of the provided vector
/// \param A The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v4_length_squared(V4F A);

/// \brief The dot product of two vectors
/// \param A,B The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v4_dot(V4F A, V4F B);

/// \brief The cross product of two vectors
/// \param A,B The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
V4F tak_v4_cross(V4F A, V4F B);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param A The vector to normalize
/// \returns The normalized version of the vector a
V4F tak_v4_normalize(V4F A);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param Start,End The two vectors in-space comprising a linear relationship
/// \param Alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
V4F tak_v4_lerp(V4F Start, V4F End, f32 Alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::V4F;

  inline V4F operator-(V4F A) { return tak_v4_neg(A); }
  inline V4F operator+(V4F A, V4F B) { return tak_v4_add(A, B); }
  inline V4F operator-(V4F A, V4F B) { return tak_v4_subtract(A, B); }
  inline V4F operator*(V4F A, f32 S) { return tak_v4_multiply(A, S); }
  inline V4F operator*(f32 S, V4F A) { return tak_v4_multiply(A, S); }
  inline V4F operator/(V4F A, f32 S) { return tak_v4_divide(A, S); }
  inline f32 Length(V4F A) { return tak_v4_length(A); }
  inline f32 LengthSquared(V4F A) { return tak_v4_length_squared(A); }
  inline f32 Dot(V4F A, V4F B) { return tak_v4_dot(A, B); }
  inline V4F Cross(V4F A, V4F B) { return tak_v4_cross(A, B); }
  inline V4F Normalize(V4F A) { return tak_v4_normalize(A); }
  inline V4F Lerp(V4F Start, V4F End, f32 Alpha) { return tak_v4_lerp(Start, End, Alpha); }
}

#endif