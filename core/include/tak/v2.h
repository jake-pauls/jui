#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;

/// \brief Two-component float vector
typedef union v2f v2f;
union v2f
{
  struct
  {
    f32 X;
    f32 Y;
  };
  f32 V[2];
};

/// Constructor
static inline v2f tak_v2(f32 X, f32 Y)
{
  v2f Result;
  Result.X = X;
  Result.Y = Y;
  return Result;
}

/// Unary minus
static inline v2f tak_v2_neg(v2f A)
{
  return tak_v2(-A.X, -A.Y);
}

/// Add
static inline v2f tak_v2_add(v2f A, v2f B)
{
  return tak_v2(A.X + B.X, A.Y + B.Y);
}

/// Subtract
static inline v2f tak_v2_subtract(v2f A, v2f B)
{
  return tak_v2(A.X - B.X, A.Y - B.Y);
}

/// Multiply scalar
static inline v2f tak_v2_multiply(v2f A, f32 S)
{
  return tak_v2(A.X * S, A.Y * S);
}

/// Divide scalar
static inline v2f tak_v2_divide(v2f A, f32 S)
{
  return tak_v2(A.X / S, A.Y / S);
}

/// \brief returns the length of the provided vector
/// \param A the vector to get the length of
/// \returns the length of the vector
f32 tak_v2_length(v2f A);

/// \brief Returns the squared length of the provided vector
/// \param A The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v2_length_squared(v2f A);

/// \brief The dot product of two vectors
/// \param A,B The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v2_dot(v2f A, v2f B);

/// \brief The cross product of two vectors
/// \param A,B The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
v2f tak_v2_cross(v2f A, v2f B);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param A The vector to normalize
/// \returns The normalized version of the vector a
v2f tak_v2_normalize(v2f A);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param Start,End The two vectors in-space comprising a linear relationship
/// \param Alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
v2f tak_v2_lerp(v2f Start, v2f End, f32 Alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::v2f;

  inline v2f operator-(v2f A) { return tak_v2_neg(A); }
  inline v2f operator+(v2f A, v2f B) { return tak_v2_add(A, B); }
  inline v2f operator-(v2f A, v2f B) { return tak_v2_subtract(A, B); }
  inline v2f operator*(v2f A, f32 S) { return tak_v2_multiply(A, S); }
  inline v2f operator*(f32 S, v2f A) { return tak_v2_multiply(A, S); }
  inline v2f operator/(v2f A, f32 S) { return tak_v2_divide(A, S); }
  inline f32 Length(v2f A) { return tak_v2_length(A); }
  inline f32 LengthSquared(v2f A) { return tak_v2_length_squared(A); }
  inline f32 Dot(v2f A, v2f B) { return tak_v2_dot(A, B); }
  inline v2f Cross(v2f A, v2f B) { return tak_v2_cross(A, B); }
  inline v2f Normalize(v2f A) { return tak_v2_normalize(A); }
  inline v2f Lerp(v2f A, v2f B, f32 T) { return tak_v2_lerp(A, B, T); }
}

#endif