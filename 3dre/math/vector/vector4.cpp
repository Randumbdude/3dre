#include "vector4.h"

vector4::vector4() : x(0.0f), y(0.0f), z(0.0f) {}

vector4::vector4(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vector4::vector4(const vector4 &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}