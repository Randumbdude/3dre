#include "vector4.h"

#include <cmath>

vector4::vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

vector4::vector4(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w)
{
}

vector4::vector4(const vector4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

float vector4::dot(const vector4 &other) const
{
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

float vector4::length_squared() const
{
    return dot(*this);
}

float vector4::length() const
{
    return std::sqrt(length_squared());
}

vector4 vector4::normalized() const
{
    const float magnitude = length();
    return magnitude == 0.0f ? vector4() : *this / magnitude;
}

vector4 vector4::operator+(const vector4 &other) const
{
    return vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

vector4 vector4::operator-(const vector4 &other) const
{
    return vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

vector4 vector4::operator*(float scalar) const
{
    return vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

vector4 vector4::operator/(float scalar) const
{
    return vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

vector4 &vector4::operator+=(const vector4 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

vector4 &vector4::operator-=(const vector4 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

vector4 &vector4::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

vector4 &vector4::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

vector4 operator*(float scalar, const vector4 &v)
{
    return v * scalar;
}
