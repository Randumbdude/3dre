#include "matrix4x4.h"

#include <cmath>

matrix4x4::matrix4x4()
    : m{
          {1.0f, 0.0f, 0.0f, 0.0f},
          {0.0f, 1.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 1.0f, 0.0f},
          {0.0f, 0.0f, 0.0f, 1.0f}}
{
}

matrix4x4::matrix4x4(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33)
    : m{
          {m00, m01, m02, m03},
          {m10, m11, m12, m13},
          {m20, m21, m22, m23},
          {m30, m31, m32, m33}}
{
}

matrix4x4 matrix4x4::identity()
{
    return matrix4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

matrix4x4 matrix4x4::zero()
{
    return matrix4x4(
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f);
}

matrix4x4 matrix4x4::translation(float x, float y, float z)
{
    matrix4x4 result = identity();
    result.m[3][0] = x;
    result.m[3][1] = y;
    result.m[3][2] = z;
    return result;
}

matrix4x4 matrix4x4::scale(float x, float y, float z)
{
    matrix4x4 result = identity();
    result.m[0][0] = x;
    result.m[1][1] = y;
    result.m[2][2] = z;
    return result;
}

matrix4x4 matrix4x4::rotation_x(float radians)
{
    const float cosine = std::cos(radians);
    const float sine = std::sin(radians);
    return matrix4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosine, sine, 0.0f,
        0.0f, -sine, cosine, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

matrix4x4 matrix4x4::rotation_y(float radians)
{
    const float cosine = std::cos(radians);
    const float sine = std::sin(radians);
    return matrix4x4(
        cosine, 0.0f, -sine, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sine, 0.0f, cosine, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

matrix4x4 matrix4x4::rotation_z(float radians)
{
    const float cosine = std::cos(radians);
    const float sine = std::sin(radians);
    return matrix4x4(
        cosine, sine, 0.0f, 0.0f,
        -sine, cosine, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

matrix4x4 matrix4x4::transposed() const
{
    matrix4x4 result = zero();
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            result.m[row][column] = m[column][row];
        }
    }
    return result;
}

vector3 matrix4x4::transform_point(const vector3 &point) const
{
    const float x = point.x * m[0][0] + point.y * m[1][0] + point.z * m[2][0] + m[3][0];
    const float y = point.x * m[0][1] + point.y * m[1][1] + point.z * m[2][1] + m[3][1];
    const float z = point.x * m[0][2] + point.y * m[1][2] + point.z * m[2][2] + m[3][2];
    const float w = point.x * m[0][3] + point.y * m[1][3] + point.z * m[2][3] + m[3][3];

    if (w != 0.0f && w != 1.0f)
        return vector3(x / w, y / w, z / w);

    return vector3(x, y, z);
}

vector3 matrix4x4::transform_direction(const vector3 &direction) const
{
    return vector3(
        direction.x * m[0][0] + direction.y * m[1][0] + direction.z * m[2][0],
        direction.x * m[0][1] + direction.y * m[1][1] + direction.z * m[2][1],
        direction.x * m[0][2] + direction.y * m[1][2] + direction.z * m[2][2]);
}

matrix4x4 matrix4x4::operator*(const matrix4x4 &other) const
{
    matrix4x4 result = zero();
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            for (int index = 0; index < 4; ++index)
            {
                result.m[row][column] += m[row][index] * other.m[index][column];
            }
        }
    }
    return result;
}
