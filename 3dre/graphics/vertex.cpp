#include "vertex.h"

vertex::vertex()
    : v(), nv(0.0f, 1.0f, 0.0f), uv()
{
}

vertex::vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
    : v(x, y, z), nv(nx, ny, nz), uv(u, v)
{
}

vertex::vertex(const vector3 &position, const vector3 &normal, const vector2 &texture_coordinates)
    : v(position), nv(normal), uv(texture_coordinates)
{
}
