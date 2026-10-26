#include "vector2.h"

vector2::vector2() : x(0.0f), y(0.0f), z(0.0f) {}

vector2::vector2(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vector2::vector2(const vector2 &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}