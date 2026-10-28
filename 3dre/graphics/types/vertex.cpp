#include "vertex.h"

vertex::vertex()
    : pos(), norm_pos(0.0f, 1.0f, 0.0f), tex_cords()
{
}

vertex::vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
    : pos(x, y, z), norm_pos(nx, ny, nz), tex_cords(u, v)
{
}

vertex::vertex(const vector3 &position, const vector3 &normal, const vector2 &texture_coordinates)
    : pos(position), norm_pos(normal), tex_cords(texture_coordinates)
{
}
