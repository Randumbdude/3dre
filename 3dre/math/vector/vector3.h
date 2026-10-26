#pragma once

class vector3
{
public:
    float x, y, z;
    vector3();
    vector3(float x, float y, float z);
    vector3(const vector3 &v);
};