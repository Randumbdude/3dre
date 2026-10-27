#include "vector3.h"

#include <cmath>

vector3::vector3() : x(0.0f), y(0.0f), z(0.0f) {}

vector3::vector3(float x, float y, float z) : x(x), y(y), z(z) {}

vector3::vector3(const vector3 &v) : x(v.x), y(v.y), z(v.z) {}

float vector3::dot(const vector3 &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

vector3 vector3::cross(const vector3 &other) const
{
    return vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
}

float vector3::length_squared() const
{
    return dot(*this);
}

float vector3::length() const
{
    return std::sqrt(length_squared());
}

vector3 vector3::normalized() const
{
    const float magnitude = length();
    return magnitude == 0.0f ? vector3() : *this / magnitude;
}

vector3 vector3::operator+(const vector3 &other) const
{
    return vector3(x + other.x, y + other.y, z + other.z);
}

vector3 vector3::operator-(const vector3 &other) const
{
    return vector3(x - other.x, y - other.y, z - other.z);
}

vector3 vector3::operator*(float scalar) const
{
    return vector3(x * scalar, y * scalar, z * scalar);
}

vector3 vector3::operator/(float scalar) const
{
    return vector3(x / scalar, y / scalar, z / scalar);
}

vector3 &vector3::operator+=(const vector3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

vector3 &vector3::operator-=(const vector3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

vector3 &vector3::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

vector3 &vector3::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

vector3 operator*(float scalar, const vector3 &v)
{
    return v * scalar;
}
