#include "engine.h"
#include "3dre.h"
#include "renderer.h"

namespace engine
{
    void render_init(void)
    {
        // add the default shaders
        renderer::add_shader("basic", L"shaders/basic_vs.cso", L"shaders/basic_ps.cso");
    }

    void render_frame(void)
    {
        // each frame add a triangle to the buffer
        renderer::add_triangle(
            vertex(-0.65f, -0.55f, 0.0f),
            vertex(0.00f, 0.65f, 0.0f),
            vertex(0.65f, -0.55f, 0.0f));
    }
};