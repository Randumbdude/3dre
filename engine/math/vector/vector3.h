#pragma once

/// A three-component vector for 3D positions, directions, and colors.
class vector3
{
public:
    float x;
    float y;
    float z;

    /// Creates the zero vector.
    vector3();

    /// Creates a vector from its x, y, and z components.
    vector3(float x, float y, float z);

    /// Copies another vector.
    vector3(const vector3 &v);

    /// Returns the dot product with another vector.
    float dot(const vector3 &other) const;

    /// Returns the cross product with another vector.
    vector3 cross(const vector3 &other) const;

    /// Returns the squared length, avoiding a square root.
    float length_squared() const;

    /// Returns the vector's Euclidean length.
    float length() const;

    /// Returns a unit length copy; returns zero when this vector has zero length.
    vector3 normalized() const;

    vector3 operator+(const vector3 &other) const;
    vector3 operator-(const vector3 &other) const;
    vector3 operator*(float scalar) const;
    vector3 operator/(float scalar) const;
    vector3 &operator+=(const vector3 &other);
    vector3 &operator-=(const vector3 &other);
    vector3 &operator*=(float scalar);
    vector3 &operator/=(float scalar);
};

/// Multiplies each vector component by a scalar.
vector3 operator*(float scalar, const vector3 &v);
