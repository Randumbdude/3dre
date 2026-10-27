#pragma once
#include "triangle.h"
#include <utility>

class mesh
{
private:

    triangle *triangles;
    size_t mesh_size;
    size_t mesh_capacity;

    void resize(size_t new_capacity)
    {
        triangle *new_tris = new triangle[new_capacity];

        for (size_t i = 0; i < mesh_size; ++i)
        {
            new_tris[i] = std::move(triangles[i]);
        }

        delete[] triangles;
        triangles = new_tris;
        mesh_capacity = new_capacity;
    }

public:
};