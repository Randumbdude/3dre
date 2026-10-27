#pragma once

/// A two-component vector for positions or directions in 2D.
class vector2
{
public:
    float x;
    float y;

    /// Creates the zero vector.
    vector2();

    /// Creates a vector from its x and y components.
    vector2(float x, float y);

    /// Copies another vector.
    vector2(const vector2 &v);

    /// Returns the dot product with another vector.
    float dot(const vector2 &other) const;

    /// Returns the squared length, avoiding a square root.
    float length_squared() const;

    /// Returns the vector's Euclidean length.
    float length() const;

    /// Returns a unit length copy; returns zero when this vector has zero length.
    vector2 normalized() const;

    vector2 operator+(const vector2 &other) const;
    vector2 operator-(const vector2 &other) const;
    vector2 operator*(float scalar) const;
    vector2 operator/(float scalar) const;
    vector2 &operator+=(const vector2 &other);
    vector2 &operator-=(const vector2 &other);
    vector2 &operator*=(float scalar);
    vector2 &operator/=(float scalar);
};

/// Multiplies each vector component by a scalar.
vector2 operator*(float scalar, const vector2 &v);
