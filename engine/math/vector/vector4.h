#pragma once

/// A four-component vector, commonly used for homogeneous 3D coordinates.
class vector4
{
public:
    float x;
    float y;
    float z;
    float w;

    /// Creates the zero vector.
    vector4();

    /// Creates a vector from its x, y, z, and w components.
    vector4(float x, float y, float z, float w);

    /// Copies another vector.
    vector4(const vector4 &v);

    /// Returns the dot product with another vector.
    float dot(const vector4 &other) const;

    /// Returns the squared length, avoiding a square root.
    float length_squared() const;

    /// Returns the vector's Euclidean length.
    float length() const;

    /// Returns a unit length copy; returns zero when this vector has zero length.
    vector4 normalized() const;

    vector4 operator+(const vector4 &other) const;
    vector4 operator-(const vector4 &other) const;
    vector4 operator*(float scalar) const;
    vector4 operator/(float scalar) const;
    vector4 &operator+=(const vector4 &other);
    vector4 &operator-=(const vector4 &other);
    vector4 &operator*=(float scalar);
    vector4 &operator/=(float scalar);
};

/// Multiplies each vector component by a scalar.
vector4 operator*(float scalar, const vector4 &v);
