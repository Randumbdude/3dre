#include "vector2.h"

#include <cmath>

vector2::vector2() : x(0.0f), y(0.0f) {}

vector2::vector2(float x, float y) : x(x), y(y) {}

vector2::vector2(const vector2 &v) : x(v.x), y(v.y) {}

float vector2::dot(const vector2 &other) const
{
    return x * other.x + y * other.y;
}

float vector2::length_squared() const
{
    return dot(*this);
}

float vector2::length() const
{
    return std::sqrt(length_squared());
}

vector2 vector2::normalized() const
{
    const float magnitude = length();
    return magnitude == 0.0f ? vector2() : *this / magnitude;
}

vector2 vector2::operator+(const vector2 &other) const
{
    return vector2(x + other.x, y + other.y);
}

vector2 vector2::operator-(const vector2 &other) const
{
    return vector2(x - other.x, y - other.y);
}

vector2 vector2::operator*(float scalar) const
{
    return vector2(x * scalar, y * scalar);
}

vector2 vector2::operator/(float scalar) const
{
    return vector2(x / scalar, y / scalar);
}

vector2 &vector2::operator+=(const vector2 &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

vector2 &vector2::operator-=(const vector2 &other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

vector2 &vector2::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

vector2 &vector2::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

vector2 operator*(float scalar, const vector2 &v)
{
    return v * scalar;
}
