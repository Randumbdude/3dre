#pragma once

class vector4
{
public:
    float x, y, z;
    vector4();
    vector4(float x, float y, float z);
    vector4(const vector4 &v);
};