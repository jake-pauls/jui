#include "tak/m44.h"
#include "tak/v3.h"

#if defined(WIN32) && defined(TAK_USE_DX_MATH)
#include <DirectXMath.h>
#endif

#include <math.h>

/// \brief Transposes the rows/columns of the input matrix
/// \param M The matrix to transpose
/// \returns The transposed version of the provided matrix
M44F tak_m44_transpose(M44F M)
{
    M44F Result = {0};

    Result.M[0][0] = M.M[0][0];
    Result.M[0][1] = M.M[1][0];
    Result.M[0][2] = M.M[2][0];
    Result.M[0][3] = M.M[3][0];

    Result.M[1][0] = M.M[0][1];
    Result.M[1][1] = M.M[1][1];
    Result.M[1][2] = M.M[2][1];
    Result.M[1][3] = M.M[3][1];

    Result.M[2][0] = M.M[0][2];
    Result.M[2][1] = M.M[1][2];
    Result.M[2][2] = M.M[2][2];
    Result.M[2][3] = M.M[3][2];

    Result.M[3][0] = M.M[0][3];
    Result.M[3][1] = M.M[1][3];
    Result.M[3][2] = M.M[2][3];
    Result.M[3][3] = M.M[3][3];

    return Result;
}

/// \brief Creates a matrix representing a translation by a specified offset
/// \param Offset The vector containing the offset
/// \returns The translate matrix corresponding to the provided offset
M44F tak_m44_translation(V3F Offset)
{
    M44F result = {0};

    result.M[0][0] = 1.0f;
    result.M[0][1] = 0.0f;
    result.M[0][2] = 0.0f;
    result.M[0][3] = 0.0f;

    result.M[1][0] = 0.0f;
    result.M[1][1] = 1.0f;
    result.M[1][2] = 0.0f;
    result.M[1][3] = 0.0f;

    result.M[2][0] = 0.0f;
    result.M[2][1] = 0.0f;
    result.M[2][2] = 1.0f;
    result.M[2][3] = 0.0f;

    result.M[3][0] = Offset.V[0];
    result.M[3][1] = Offset.V[1];
    result.M[3][2] = Offset.V[2];
    result.M[3][3] = 1.0f;

    return result;
}

/// \brief Creates a perspective matrix using a left-handed basis
M44F tak_m44_perspective_left_handed(f32 FOV, f32 Aspect, f32 NearZ, f32 FarZ)
{
    M44F result = {0};

#if defined(WIN32) && defined(TAK_USE_DX_MATH)

    DirectX::XMFLOAT4X4 dxResult;

    DirectX::XMMATRIX perspective =
        DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(FOV),
            Aspect,
            NearZ,
            FarZ
        );

    DirectX::XMStoreFloat4x4(&dxResult, perspective);

    result.M[0][0] = dxResult.m[0][0];
    result.M[0][1] = dxResult.m[0][1];
    result.M[0][2] = dxResult.m[0][2];
    result.M[0][3] = dxResult.m[0][3];

    result.M[1][0] = dxResult.m[1][0];
    result.M[1][1] = dxResult.m[1][1];
    result.M[1][2] = dxResult.m[1][2];
    result.M[1][3] = dxResult.m[1][3];

    result.M[2][0] = dxResult.m[2][0];
    result.M[2][1] = dxResult.m[2][1];
    result.M[2][2] = dxResult.m[2][2];
    result.M[2][3] = dxResult.m[2][3];

    result.M[3][0] = dxResult.m[3][0];
    result.M[3][1] = dxResult.m[3][1];
    result.M[3][2] = dxResult.m[3][2];
    result.M[3][3] = dxResult.m[3][3];

#else

    const float sinFov = sinf(0.5f * FOV);
    const float cosFov = cosf(0.5f * FOV);

    const float height = cosFov / sinFov;
    const float width  = height / Aspect;
    const float fRange = FarZ / (FarZ - NearZ);

    result.M[0][0] = width;
    result.M[0][1] = 0.0f;
    result.M[0][2] = 0.0f;
    result.M[0][3] = 0.0f;

    result.M[1][0] = 0.0f;
    result.M[1][1] = height;
    result.M[1][2] = 0.0f;
    result.M[1][3] = 0.0f;

    result.M[2][0] = 0.0f;
    result.M[2][1] = 0.0f;
    result.M[2][2] = fRange;
    result.M[2][3] = 1.0f;

    result.M[3][0] = 0.0f;
    result.M[3][1] = 0.0f;
    result.M[3][2] = -fRange * NearZ;
    result.M[3][3] = 0.0f;

#endif

    return result;
}

/// \brief Creates a view matrix using a left-handed basis
M44F tak_m44_look_at_left_handed(V3F Eye, V3F Focus, V3F Up)
{
    M44F result = {0};

#if defined(WIN32) && defined(TAK_USE_DX_MATH)

    DirectX::XMFLOAT4X4 dxResult;

    DirectX::XMVECTOR dxEye   = DirectX::XMVectorSet(Eye.X, Eye.Y, Eye.Z, 0.0f);
    DirectX::XMVECTOR dxFocus = DirectX::XMVectorSet(Focus.X, Focus.Y, Focus.Z, 0.0f);
    DirectX::XMVECTOR dxUp    = DirectX::XMVectorSet(Up.X, Up.Y, Up.Z, 0.0f);

    DirectX::XMMATRIX lookAt = DirectX::XMMatrixLookAtLH(dxEye, dxFocus, dxUp);

    DirectX::XMStoreFloat4x4(&dxResult, lookAt);

    result.M[0][0] = dxResult.m[0][0];
    result.M[0][1] = dxResult.m[0][1];
    result.M[0][2] = dxResult.m[0][2];
    result.M[0][3] = dxResult.m[0][3];

    result.M[1][0] = dxResult.m[1][0];
    result.M[1][1] = dxResult.m[1][1];
    result.M[1][2] = dxResult.m[1][2];
    result.M[1][3] = dxResult.m[1][3];

    result.M[2][0] = dxResult.m[2][0];
    result.M[2][1] = dxResult.m[2][1];
    result.M[2][2] = dxResult.m[2][2];
    result.M[2][3] = dxResult.m[2][3];

    result.M[3][0] = dxResult.m[3][0];
    result.M[3][1] = dxResult.m[3][1];
    result.M[3][2] = dxResult.m[3][2];
    result.M[3][3] = dxResult.m[3][3];

#else

    // NOTE(jake): Invert orthogonality for left-handed coordinate system.
    const V3F f = tak_v3_normalize(tak_v3_subtract(Focus, Eye));
    const V3F s = tak_v3_normalize(tak_v3_cross(Up, f));
    const V3F u = tak_v3_cross(f, s);

    result.M[0][0] = s.V[0];
    result.M[0][1] = u.V[0];
    result.M[0][2] = f.V[0];
    result.M[0][3] = 0.0f;

    result.M[1][0] = s.V[1];
    result.M[1][1] = u.V[1];
    result.M[1][2] = f.V[1];
    result.M[1][3] = 0.0f;

    result.M[2][0] = s.V[2];
    result.M[2][1] = u.V[2];
    result.M[2][2] = f.V[2];
    result.M[2][3] = 0.0f;

    // NOTE(jake): The translation row is calculated by projecting the eye position onto each axis.
    result.M[3][0] = -tak_v3_dot(s, Eye);
    result.M[3][1] = -tak_v3_dot(u, Eye);
    result.M[3][2] = -tak_v3_dot(f, Eye);
    result.M[3][3] = 1.0f;

#endif

    return result;
}