#pragma once
#include "vertex.h"

/// A triangle made from three vertices.
class triangle
{
public:
    vertex vertices[3];

    /// Creates a triangle with three default vertices.
    triangle();

    /// Creates a triangle from its three vertices.
    triangle(const vertex &a, const vertex &b, const vertex &c);
};
