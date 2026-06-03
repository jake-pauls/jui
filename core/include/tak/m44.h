#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef float f32;
typedef union v3f v3f;

/// \brief 4x4 float matrix
typedef struct m44f m44f;
struct m44f
{
  f32 M[4][4];
};

/// \brief Transposes the rows/columns of the input matrix
/// \param M The matrix to transpose
/// \returns The transposed version of the provided matrix
m44f tak_m44_transpose(m44f M);

/// \brief Creates a matrix representing a translation by a specified offset
/// \param Offset The vector containing the offset
/// \returns The translation matrix corresponding to the provided offset
m44f tak_m44_translation(v3f Offset);

// TODO(jake): Rotate
// TODO(jake): Scale

/// \brief Creates a perspective matrix using a left-handed basis
m44f tak_m44_perspective_left_handed(f32 FOV, f32 Aspect, f32 NearZ, f32 FarZ);

/// \brief Creates a view matrix using a left-handed basis
m44f tak_m44_look_at_left_handed(v3f Eye, v3f Focus, v3f Up);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace tak
{
  using ::M44F;

  inline m44f MatrixTranspose(m44f M) { return tak_m44_transpose(M); }
  inline m44f MatrixTranslation(v3f Offset) { return tak_m44_translation(Offset); }
  inline m44f MatrixPerspectiveLH(f32 FOV, f32 Aspect, f32 NearZ, f32 FarZ) { return tak_m44_perspective_left_handed(FOV, Aspect, NearZ, FarZ); }
  inline m44f MatrixLookAtLH(v3f Eye, v3f Focus, v3f Up) { return tak_m44_look_at_left_handed(Eye, Focus, Up); }
}

#endif
