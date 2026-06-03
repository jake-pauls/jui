#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;
typedef double f64;

/// \brief Three-component float vector
typedef union v3f v3f;
union v3f
{
  struct
  {
    f32 X;
    f32 Y;
    f32 Z;
  };
  f32 V[3];
};

/// Constructors/helpers
static inline v3f tak_v3(f32 X, f32 Y, f32 Z)
{
  v3f Result;
  Result.X = X;
  Result.Y = Y;
  Result.Z = Z;
  return Result;
}

/// Unary Negation
static inline v3f tak_v3_neg(v3f A)
{
  return tak_v3(-A.X, -A.Y, -A.Z);
}

/// Addition
static inline v3f tak_v3_add(v3f A, v3f B)
{
  return tak_v3(
      A.X + B.X,
      A.Y + B.Y,
      A.Z + B.Z
      );
}

/// Subtraction
static inline v3f tak_v3_subtract(v3f A, v3f B)
{
  return tak_v3(
      A.X - B.X,
      A.Y - B.Y,
      A.Z - B.Z
      );
}

/// Scalar multiply
static inline v3f tak_v3_multiply(v3f A, f32 S)
{
  return tak_v3(
      A.X * S,
      A.Y * S,
      A.Z * S
      );
}

/// Scalar divide
static inline v3f tak_v3_divide(v3f A, f32 S)
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
f32 tak_v3_length(v3f A);

/// \brief Returns the squared length of the provided vector
/// \param A The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v3_length_squared(v3f A);

/// \brief The dot product of two vectors
/// \param A,B The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v3_dot(v3f A, v3f B);

/// \brief The cross product of two vectors
/// \param A,B The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
v3f tak_v3_cross(v3f A, v3f B);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param A The vector to normalize
/// \returns The normalized version of the vector a
v3f tak_v3_normalize(v3f A);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param Start,End The two vectors in-space comprising a linear relationship
/// \param Alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
v3f take_v3_lerp(v3f Start, v3f End, f32 Alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::v3f;

  inline v3f operator-(v3f A) { return tak_v3_neg(A); }
  inline v3f operator+(v3f A, v3f B) { return tak_v3_add(A, B); }
  inline v3f operator-(v3f A, v3f B) { return tak_v3_subtract(A, B); }
  inline v3f operator*(v3f A, f32 S) { return tak_v3_multiply(A, S); }
  inline v3f operator*(f32 S, v3f A) { return tak_v3_multiply(A, S); }
  inline v3f operator/(v3f A, f32 S) { return tak_v3_divide(A, S); }
  inline f32 Length(v3f A) { return tak_v3_length(A); }
  inline f32 LengthSquared(v3f A) { return tak_v3_length_squared(A); }
  inline f32 Dot(v3f A, v3f B) { return tak_v3_dot(A, B); }
  inline v3f Cross(v3f A, v3f B) { return tak_v3_cross(A, B); }
  inline v3f Normalize(v3f A) { return tak_v3_normalize(A); }
  inline v3f Lerp(v3f Start, v3f End, f32 Alpha) { return take_v3_lerp(Start, End, Alpha); }
}

#endif