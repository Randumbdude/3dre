#pragma once

#include "matrix4x4.h"
#include "vector3.h"

class camera
{
public:
    vector3 position{ 0.0f, 0.0f, -5.0f };
    vector3 target{ 0.0f, 0.0f,  0.0f };
    vector3 up{ 0.0f, 1.0f,  0.0f };

    float fov = 1.04719755f; // 60 degrees in radians
    float aspect_ratio = 16.0f / 9.0f;
    float near_plane = 0.1f;
    float far_plane = 1000.0f;

    matrix4x4 get_view_matrix() const
    {
        return matrix4x4::look_at_lh(position, target, up);
    }

    matrix4x4 get_projection_matrix() const
    {
        return matrix4x4::perspective_fov_lh(
            fov,
            aspect_ratio,
            near_plane,
            far_plane
        );
    }

    void set_aspect_ratio(float width, float height)
    {
        if (height > 0.0f)
            aspect_ratio = width / height;
    }
};