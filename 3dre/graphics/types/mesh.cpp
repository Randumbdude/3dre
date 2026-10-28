#include "mesh.h"

void mesh::add_triangle(const triangle &value)
{
    triangles_.push_back(value);
}

void mesh::add_triangle(const vertex &a, const vertex &b, const vertex &c)
{
    triangles_.emplace_back(a, b, c);
}

void mesh::clear()
{
    triangles_.clear();
}

std::size_t mesh::triangle_count() const
{
    return triangles_.size();
}

std::size_t mesh::vertex_count() const
{
    return triangles_.size() * 3;
}

const std::vector<triangle> &mesh::get_triangles() const
{
    return triangles_;
}

std::vector<triangle> &mesh::get_triangles()
{
    return triangles_;
}

std::vector<vertex> mesh::get_vertices() const
{
    std::vector<vertex> result;
    result.reserve(vertex_count());

    for (const triangle &current_triangle : triangles_)
    {
        for (const vertex &current_vertex : current_triangle.vertices)
        {
            result.push_back(current_vertex);
        }
    }

    return result;
}
