#pragma once

#include "vector3.h"

/// A 4 by 4 matrix for 3D transformations.
///
/// Elements are stored as m[row][column]. Vectors are treated as row vectors
/// and multiplied on the left (v * matrix). Translation is stored in row 3.
/// With this convention, transformations are commonly combined as
/// scale * rotation * translation.
class matrix4x4
{
public:
    /// Matrix elements, indexed as m[row][column].
    float m[4][4];

    /// Creates an identity matrix.
    matrix4x4();

    /// Creates a matrix from sixteen row-major values.
    matrix4x4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33);

    /// Returns an identity matrix (no transformation).
    static matrix4x4 identity();

    /// Returns a matrix whose elements are all zero.
    static matrix4x4 zero();

    /// Creates a translation matrix. The offset is applied to points, not directions.
    static matrix4x4 translation(float x, float y, float z);

    /// Creates a scale matrix with separate factors for each axis.
    static matrix4x4 scale(float x, float y, float z);

    /// Creates an X-axis rotation. The angle is in radians.
    static matrix4x4 rotation_x(float radians);

    /// Creates a Y-axis rotation. The angle is in radians.
    static matrix4x4 rotation_y(float radians);

    /// Creates a Z-axis rotation. The angle is in radians.
    static matrix4x4 rotation_z(float radians);

    /// Returns the transpose, swapping rows and columns.
    matrix4x4 transposed() const;

    /// Transforms a position, including translation and perspective divide.
    vector3 transform_point(const vector3 &point) const;

    /// Transforms a direction, ignoring translation.
    vector3 transform_direction(const vector3 &direction) const;

    /// Multiplies two matrices in left-to-right row-vector transform order.
    matrix4x4 operator*(const matrix4x4 &other) const;
};
