#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef float f32;

/// \brief Four-component float vector
typedef union v4f v4f;
union v4f
{
  struct
  {
    f32 X;
    f32 Y;
    f32 Z;
    f32 W;
  };
  f32 V[4];
};

/// \brief Creates a four-component vector
static inline v4f tak_v4(f32 X, f32 Y, f32 Z, f32 W)
{
  v4f Result;
  Result.X = X;
  Result.Y = Y;
  Result.Z = Z;
  Result.W = W;
  return Result;
}

/// \brief Negates the provided vector
static inline v4f tak_v4_neg(v4f A)
{
  return tak_v4(
      -A.X,
      -A.Y,
      -A.Z,
      -A.W
      );
}

/// \brief Adds two vectors
static inline v4f tak_v4_add(v4f A, v4f B)
{
  return tak_v4(
      A.X + B.X,
      A.Y + B.Y,
      A.Z + B.Z,
      A.W + B.W
      );
}

/// \brief Subtracts two vectors
static inline v4f tak_v4_subtract(v4f A, v4f B)
{
  return tak_v4(
      A.X - B.X,
      A.Y - B.Y,
      A.Z - B.Z,
      A.W - B.W
      );
}

/// \brief Multiplies a vector by a scalar
static inline v4f tak_v4_multiply(v4f A, f32 S)
{
  return tak_v4(
      A.X * S,
      A.Y * S,
      A.Z * S,
      A.W * S
      );
}

/// \brief Divides a vector by a scalar
static inline v4f tak_v4_divide(v4f A, f32 S)
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
f32 tak_v4_length(v4f A);

/// \brief Returns the squared length of the provided vector
/// \param A The vector to get the squared length of
/// \returns The squared length of the vector
f32 tak_v4_length_squared(v4f A);

/// \brief The dot product of two vectors
/// \param A,B The two vectors to calculate the dot product of
/// \returns The dot product of the provided vectors a and b
f32 tak_v4_dot(v4f A, v4f B);

/// \brief The cross product of two vectors
/// \param A,B The two vectors to calculate the cross product of
/// \returns The cross product of the provided vectors a and b
v4f tak_v4_cross(v4f A, v4f B);

/// \brief Returns the normalized version of the provided vector (unit vector)
/// \param A The vector to normalize
/// \returns The normalized version of the vector a
v4f tak_v4_normalize(v4f A);

/// \brief Returns a vector on the linear function of a starting vector and an ending vector at a provided alpha
/// \param Start,End The two vectors in-space comprising a linear relationship
/// \param Alpha The value on the linear relationship to return
/// \returns Value on the linear function of the starting vector and the ending vector at the provided alpha
v4f tak_v4_lerp(v4f Start, v4f End, f32 Alpha);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::v4f;

  inline v4f operator-(v4f A) { return tak_v4_neg(A); }
  inline v4f operator+(v4f A, v4f B) { return tak_v4_add(A, B); }
  inline v4f operator-(v4f A, v4f B) { return tak_v4_subtract(A, B); }
  inline v4f operator*(v4f A, f32 S) { return tak_v4_multiply(A, S); }
  inline v4f operator*(f32 S, v4f A) { return tak_v4_multiply(A, S); }
  inline v4f operator/(v4f A, f32 S) { return tak_v4_divide(A, S); }
  inline f32 Length(v4f A) { return tak_v4_length(A); }
  inline f32 LengthSquared(v4f A) { return tak_v4_length_squared(A); }
  inline f32 Dot(v4f A, v4f B) { return tak_v4_dot(A, B); }
  inline v4f Cross(v4f A, v4f B) { return tak_v4_cross(A, B); }
  inline v4f Normalize(v4f A) { return tak_v4_normalize(A); }
  inline v4f Lerp(v4f Start, v4f End, f32 Alpha) { return tak_v4_lerp(Start, End, Alpha); }
}

#endif