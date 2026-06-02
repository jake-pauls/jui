#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef float f32;
typedef union V3F V3F;

/// \brief 4x4 float matrix
typedef struct M44F
{
  f32 M[4][4];
} M44F;

/// \brief Transposes the rows/columns of the input matrix
/// \param M The matrix to transpose
/// \returns The transposed version of the provided matrix
M44F tak_m44_transpose(M44F M);

/// \brief Creates a matrix representing a translation by a specified offset
/// \param Offset The vector containing the offset
/// \returns The translation matrix corresponding to the provided offset
M44F tak_m44_translation(V3F Offset);

// TODO(jake): Rotate
// TODO(jake): Scale

/// \brief Creates a perspective matrix using a left-handed basis
M44F tak_m44_perspective_left_handed(f32 FOV, f32 Aspect, f32 NearZ, f32 FarZ);

/// \brief Creates a view matrix using a left-handed basis
M44F tak_m44_look_at_left_handed(V3F Eye, V3F Focus, V3F Up);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::M44F;

  inline M44F MatrixTranspose(M44F M) { return tak_m44_transpose(M); }
  inline M44F MatrixTranslation(V3F Offset) { return tak_m44_translation(Offset); }
  inline M44F MatrixPerspectiveLH(f32 FOV, f32 Aspect, f32 NearZ, f32 FarZ) { return tak_m44_perspective_left_handed(FOV, Aspect, NearZ, FarZ); }
  inline M44F MatrixLookAtLH(V3F Eye, V3F Focus, V3F Up) { return tak_m44_look_at_left_handed(Eye, Focus, Up); }
}

#endif
