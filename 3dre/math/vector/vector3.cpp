#include "vector3.h"

vector3::vector3() : x(0.0f), y(0.0f), z(0.0f) {}

vector3::vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vector3::vector3(const vector3 &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}