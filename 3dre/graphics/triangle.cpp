#include "triangle.h"

triangle::triangle() = default;

triangle::triangle(const vertex &a, const vertex &b, const vertex &c)
    : vertices{a, b, c}
{
}
