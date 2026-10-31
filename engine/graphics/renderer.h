#pragma once
#include "3dre.h"
#include "shader.h"
#include "camera.h"

#include <unordered_map>

namespace renderer
{
    // these 4 variables are the core foundation of rendering with directx.
    // together they handle: hardware management, command execution, screen buffers, and drawing targets.
    extern ComPtr<ID3D11Device> g_device;                  // is the art supply store where you buy your canvas and paint brushes
    extern ComPtr<ID3D11DeviceContext> g_context;          // is the artist holding the brushes and doing the actual painting.
    extern ComPtr<IDXGISwapChain> g_swap_chain;            // is the gallery assistant who flips the easel around to show the audience the finished painting while handing the artist a fresh canvas.
    extern ComPtr<ID3D11RenderTargetView> g_render_target; // is the canvas currently sitting on the easel being painted.

    inline camera main_camera;

    inline std::unordered_map<std::string, shader> shaders;

    // add shader should only be called once per shader file. once loaded it doesnt need to be loaded again unless unloaded. (unloading hasnt been implemented...)
    bool add_shader(const std::string &name, const wchar_t *vertex_file, const wchar_t *pixel_file);

    void add_triangle(const vertex &a, const vertex &b, const vertex &c, const std::string &shader = "basic");

    void render_main(const camera &active_camera);
};