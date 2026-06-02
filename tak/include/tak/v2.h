#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;

/// \brief Two-component float vector
typedef union V2F
{
  struct
  {
    f32 X;
    f32 Y;
  };
  f32 V[2];
} V2F;

/// Constructor
static inline V2F tak_v2(f32 X, f32 Y)
{
  V2F Result;
  Result.X = X;
  Result.Y = Y;
  return Result;
}

/// Unary minus
static inline V2F tak_v2_neg(V2F A)
{
  return tak_v2(-A.X, -A.Y);
}

/// Add
static inline V2F tak_v2_add(V2F A, V2F B)
{
  return tak_v2(A.X + B.X, A.Y + B.Y);
}

/// Subtract
static inline V2F tak_v2_subtract(V2F A, V2F B)
{
  return tak_v2(A.X - B.X, A.Y - B.Y);
}

/// Multiply scalar
static inline V2F tak_v2_multiply(V2F A, f32 S)
{
  return tak_v2(A.X * S, A.Y * S);
}

/// Divide scalar
static inline V2F tak_v2_divide(V2F A, f32 S)
{
  return tak_v2(A.X / S, A.Y / S);
}

/// \brief returns the length of the provided vector
/// \param A the vector to get the length of
/// \returns the length of the vector
f32 tak_v2_length(V2F A);

/// \brief Returns the squared length of the provided vector
/// \param A The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v2_length_squared(V2F A);

/// \brief The dot product of two vectors
/// \param A,B The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v2_dot(V2F A, V2F B);

/// \brief The cross product of two vectors
/// \param A,B The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
V2F tak_v2_cross(V2F A, V2F B);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param A The vector to normalize
/// \returns The normalized version of the vector a
V2F tak_v2_normalize(V2F A);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param Start,End The two vectors in-space comprising a linear relationship
/// \param Alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
V2F tak_v2_lerp(V2F Start, V2F End, f32 Alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::V2f;

  inline V2F operator-(V2F A) { return tak_v2_neg(A); }
  inline V2F operator+(V2F A, V2F B) { return tak_v2_add(A, B); }
  inline V2F operator-(V2F A, V2F B) { return tak_v2_subtract(A, B); }
  inline V2F operator*(V2F A, f32 S) { return tak_v2_multiply(A, S); }
  inline V2F operator*(f32 S, V2F A) { return tak_v2_multiply(A, S); }
  inline V2F operator/(V2F A, f32 S) { return tak_v2_divide(A, S); }
  inline f32 Length(V2F A) { return tak_v2_length(A); }
  inline f32 LengthSquared(V2F A) { return tak_v2_length_squared(A); }
  inline f32 Dot(V2F A, V2F B) { return tak_v2_dot(A, B); }
  inline V2F Cross(V2F A, V2F B) { return tak_v2_cross(A, B); }
  inline V2F Normalize(V2F A) { return tak_v2_normalize(A); }
  inline V2F Lerp(V2F A, V2F B, f32 T) { return tak_v2_lerp(A, B, T); }
}

#endif