#pragma once

#include "triangle.h"

#include <cstddef>
#include <vector>

/// CPU-side triangle mesh data, ready to be converted into a GPU vertex buffer.
class mesh
{
public:
    /// Adds a copy of a triangle to the mesh.
    void add_triangle(const triangle &value);

    /// Creates and adds a triangle from three vertices.
    void add_triangle(const vertex &a, const vertex &b, const vertex &c);

    /// Removes all triangles from the mesh.
    void clear();

    /// Returns the number of triangles in the mesh.
    std::size_t triangle_count() const;

    /// Returns the number of vertices when triangles are drawn independently.
    std::size_t vertex_count() const;

    /// Returns a read-only view of the mesh's triangles.
    const std::vector<triangle> &get_triangles() const;

    /// Returns a mutable view of the mesh's triangles.
    std::vector<triangle> &get_triangles();

    /// Copies triangle vertices into a contiguous list suitable for a vertex buffer.
    std::vector<vertex> get_vertices() const;

private:
    std::vector<triangle> triangles_;
};
