#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef float f32;
typedef union v3f v3f;

/// \brief 4x4 float matrix
typedef struct m44f
{
  f32 m[4][4];
} m44f;

/// \brief Transposes the rows/columns of the input matrix
/// \param m The matrix to transpose
/// \returns The transposed version of the provided matrix
m44f tak_m44_transpose(m44f m);

/// \brief Creates a matrix representing a translation by a specified offset
/// \param offset The vector containing the offset
/// \returns The translation matrix corresponding to the provided offset
m44f tak_m44_translation(v3f offset);

// TODO(jake): Rotate
// TODO(jake): Scale

/// \brief Creates a perspective matrix using a left-handed basis
m44f tak_m44_perspective_left_handed(f32 fov, f32 aspect, f32 nearZ, f32 farZ);

/// \brief Creates a view matrix using a left-handed basis
m44f tak_m44_look_at_left_handed(v3f eye, v3f focus, v3f up);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::M44F;

  inline m44f MatrixTranspose(m44f m) { return tak_m44_transpose(m); }
  inline m44f MatrixTranslation(v3f offset) { return tak_m44_translation(offset); }
  inline m44f MatrixPerspectiveLH(f32 fov, f32 aspect, f32 nearZ, f32 farZ) { return tak_m44_perspective_left_handed(fov, aspect, nearZ, farZ); }
  inline m44f MatrixLookAtLH(v3f eye, v3f focus, v3f up) { return tak_m44_look_at_left_handed(eye, focus, up); }
}

#endif
