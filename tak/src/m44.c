#include "tak/m44.h"
#include "tak/v3.h"

#if defined(WIN32) && defined(TAK_USE_DX_MATH)
#include <DirectXMath.h>
#endif

#include <math.h>

/// \brief Transposes the rows/columns of the input matrix
/// \param m The matrix to transpose
/// \returns The transposed version of the provided matrix
m44f tak_m44_transpose(m44f m)
{
    m44f result = {0};

    result.m[0][0] = m.m[0][0];
    result.m[0][1] = m.m[1][0];
    result.m[0][2] = m.m[2][0];
    result.m[0][3] = m.m[3][0];

    result.m[1][0] = m.m[0][1];
    result.m[1][1] = m.m[1][1];
    result.m[1][2] = m.m[2][1];
    result.m[1][3] = m.m[3][1];

    result.m[2][0] = m.m[0][2];
    result.m[2][1] = m.m[1][2];
    result.m[2][2] = m.m[2][2];
    result.m[2][3] = m.m[3][2];

    result.m[3][0] = m.m[0][3];
    result.m[3][1] = m.m[1][3];
    result.m[3][2] = m.m[2][3];
    result.m[3][3] = m.m[3][3];

    return result;
}

/// \brief Creates a matrix representing a translation by a specified offset
/// \param offset The vector containing the offset
/// \returns The translate matrix corresponding to the provided offset
m44f tak_m44_translation(v3f offset)
{
    m44f result = {0};

    result.m[0][0] = 1.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f;
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f;
    result.m[2][3] = 0.0f;

    result.m[3][0] = offset.v[0];
    result.m[3][1] = offset.v[1];
    result.m[3][2] = offset.v[2];
    result.m[3][3] = 1.0f;

    return result;
}

/// \brief Creates a perspective matrix using a left-handed basis
m44f tak_m44_perspective_left_handed(f32 fov, f32 aspect, f32 nearZ, f32 farZ)
{
    m44f result = {0};

#if defined(WIN32) && defined(TAK_USE_DX_MATH)

    DirectX::XMFLOAT4X4 dxResult;

    DirectX::XMMATRIX perspective =
        DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(fov),
            aspect,
            nearZ,
            farZ
        );

    DirectX::XMStoreFloat4x4(&dxResult, perspective);

    result.m[0][0] = dxResult.m[0][0];
    result.m[0][1] = dxResult.m[0][1];
    result.m[0][2] = dxResult.m[0][2];
    result.m[0][3] = dxResult.m[0][3];

    result.m[1][0] = dxResult.m[1][0];
    result.m[1][1] = dxResult.m[1][1];
    result.m[1][2] = dxResult.m[1][2];
    result.m[1][3] = dxResult.m[1][3];

    result.m[2][0] = dxResult.m[2][0];
    result.m[2][1] = dxResult.m[2][1];
    result.m[2][2] = dxResult.m[2][2];
    result.m[2][3] = dxResult.m[2][3];

    result.m[3][0] = dxResult.m[3][0];
    result.m[3][1] = dxResult.m[3][1];
    result.m[3][2] = dxResult.m[3][2];
    result.m[3][3] = dxResult.m[3][3];

#else

    const float sinFov = sinf(0.5f * fov);
    const float cosFov = cosf(0.5f * fov);

    const float height = cosFov / sinFov;
    const float width  = height / aspect;
    const float fRange = farZ / (farZ - nearZ);

    result.m[0][0] = width;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = height;
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = fRange;
    result.m[2][3] = 1.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = -fRange * nearZ;
    result.m[3][3] = 0.0f;

#endif

    return result;
}

/// \brief Creates a view matrix using a left-handed basis
m44f tak_m44_look_at_left_handed(v3f eye, v3f focus, v3f up)
{
    m44f result = {0};

#if defined(WIN32) && defined(TAK_USE_DX_MATH)

    DirectX::XMFLOAT4X4 dxResult;

    DirectX::XMVECTOR dxEye   = DirectX::XMVectorSet(eye.x, eye.y, eye.z, 0.0f);
    DirectX::XMVECTOR dxFocus = DirectX::XMVectorSet(focus.x, focus.y, focus.z, 0.0f);
    DirectX::XMVECTOR dxUp    = DirectX::XMVectorSet(up.x, up.y, up.z, 0.0f);

    DirectX::XMMATRIX lookAt = DirectX::XMMatrixLookAtLH(dxEye, dxFocus, dxUp);

    DirectX::XMStoreFloat4x4(&dxResult, lookAt);

    result.m[0][0] = dxResult.m[0][0];
    result.m[0][1] = dxResult.m[0][1];
    result.m[0][2] = dxResult.m[0][2];
    result.m[0][3] = dxResult.m[0][3];

    result.m[1][0] = dxResult.m[1][0];
    result.m[1][1] = dxResult.m[1][1];
    result.m[1][2] = dxResult.m[1][2];
    result.m[1][3] = dxResult.m[1][3];

    result.m[2][0] = dxResult.m[2][0];
    result.m[2][1] = dxResult.m[2][1];
    result.m[2][2] = dxResult.m[2][2];
    result.m[2][3] = dxResult.m[2][3];

    result.m[3][0] = dxResult.m[3][0];
    result.m[3][1] = dxResult.m[3][1];
    result.m[3][2] = dxResult.m[3][2];
    result.m[3][3] = dxResult.m[3][3];

#else

    // NOTE(jake): Invert orthogonality for left-handed coordinate system.
    const v3f f = tak_v3_normalize(tak_v3_subtract(focus, eye));
    const v3f s = tak_v3_normalize(tak_v3_cross(up, f));
    const v3f u = tak_v3_cross(f, s);

    result.m[0][0] = s.v[0];
    result.m[0][1] = u.v[0];
    result.m[0][2] = f.v[0];
    result.m[0][3] = 0.0f;

    result.m[1][0] = s.v[1];
    result.m[1][1] = u.v[1];
    result.m[1][2] = f.v[1];
    result.m[1][3] = 0.0f;

    result.m[2][0] = s.v[2];
    result.m[2][1] = u.v[2];
    result.m[2][2] = f.v[2];
    result.m[2][3] = 0.0f;

    // NOTE(jake): The translation row is calculated by projecting the eye position onto each axis.
    result.m[3][0] = -tak_v3_dot(s, eye);
    result.m[3][1] = -tak_v3_dot(u, eye);
    result.m[3][2] = -tak_v3_dot(f, eye);
    result.m[3][3] = 1.0f;

#endif

    return result;
}