#pragma once
#include "vector3.h"
#include "vector2.h"

class vertex
{
public:
    /// Position in object space.
    vector3 v;

    /// Surface normal in object space (defaults to up).
    vector3 nv;

    /// Texture coordinates (UV).
    vector2 uv;

    /// Creates a vertex at the origin with an up-facing normal and zero UVs.
    vertex();

    /// Creates a vertex with position, optional normal, and optional UV values.
    vertex(float x, float y, float z, float nx = 0.0f, float ny = 1.0f, float nz = 0.0f, float u = 0.0f, float v = 0.0f);

    /// Creates a vertex from position, normal, and UV vectors.
    vertex(const vector3 &position, const vector3 &normal = vector3(0.0f, 1.0f, 0.0f), const vector2 &texture_coordinates = vector2());
};
