#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;

/// \brief Three-component float vector
typedef union V3F
{
  struct
  {
    f32 X;
    f32 Y;
    f32 Z;
  };
  f32 V[3];
} V3F;

/// Constructors/helpers
static inline V3F tak_v3(f32 X, f32 Y, f32 Z)
{
  V3F Result;
  Result.X = X;
  Result.Y = Y;
  Result.Z = Z;
  return Result;
}

/// Unary Negation
static inline V3F tak_v3_neg(V3F A)
{
  return tak_v3(-A.X, -A.Y, -A.Z);
}

/// Addition
static inline V3F tak_v3_add(V3F A, V3F B)
{
  return tak_v3(
      A.X + B.X,
      A.Y + B.Y,
      A.Z + B.Z
      );
}

/// Subtraction
static inline V3F tak_v3_subtract(V3F A, V3F B)
{
  return tak_v3(
      A.X - B.X,
      A.Y - B.Y,
      A.Z - B.Z
      );
}

/// Scalar multiply
static inline V3F tak_v3_multiply(V3F A, f32 S)
{
  return tak_v3(
      A.X * S,
      A.Y * S,
      A.Z * S
      );
}

/// Scalar divide
static inline V3F tak_v3_divide(V3F A, f32 S)
{
  return tak_v3(
      A.X / S,
      A.Y / S,
      A.Z / S
      );
}

/// \brief Returns the length of the provided vector
/// \param A The vector to get the length of
/// \returns The length of the vector
f32 tak_v3_length(V3F A);

/// \brief Returns the squared length of the provided vector
/// \param A The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v3_length_squared(V3F A);

/// \brief The dot product of two vectors
/// \param A,B The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v3_dot(V3F A, V3F B);

/// \brief The cross product of two vectors
/// \param A,B The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
V3F tak_v3_cross(V3F A, V3F B);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param A The vector to normalize
/// \returns The normalized version of the vector a
V3F tak_v3_normalize(V3F A);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param Start,End The two vectors in-space comprising a linear relationship
/// \param Alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
V3F take_v3_lerp(V3F Start, V3F End, f32 Alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::V3F;

  inline V3F operator-(V3F A) { return tak_v3_neg(A); }
  inline V3F operator+(V3F A, V3F B) { return tak_v3_add(A, B); }
  inline V3F operator-(V3F A, V3F B) { return tak_v3_subtract(A, B); }
  inline V3F operator*(V3F A, f32 S) { return tak_v3_multiply(A, S); }
  inline V3F operator*(f32 S, V3F A) { return tak_v3_multiply(A, S); }
  inline V3F operator/(V3F A, f32 S) { return tak_v3_divide(A, S); }
  inline f32 Length(V3F A) { return tak_v3_length(A); }
  inline f32 LengthSquared(V3F A) { return tak_v3_length_squared(A); }
  inline f32 Dot(V3F A, V3F B) { return tak_v3_dot(A, B); }
  inline V3F Cross(V3F A, V3F B) { return tak_v3_cross(A, B); }
  inline V3F Normalize(V3F A) { return tak_v3_normalize(A); }
  inline V3F Lerp(V3F Start, V3F End, f32 Alpha) { return take_v3_lerp(Start, End, Alpha); }
}

#endif