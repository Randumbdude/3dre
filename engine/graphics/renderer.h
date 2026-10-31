#pragma once
#include "3dre.h"
#include "shader.h"
#include "camera.h"

#include <unordered_map>

namespace renderer
{
    extern ComPtr<ID3D11Device> g_device;
    extern ComPtr<ID3D11DeviceContext> g_context;
    extern ComPtr<IDXGISwapChain> g_swap_chain;
    extern ComPtr<ID3D11RenderTargetView> g_render_target;

    inline camera main_camera;

    inline std::unordered_map<std::string, shader> shaders;

    // add shader should only be called once per shader file. once loaded it doesnt need to be loaded again unless unloaded. (unloading hasnt been implemented...)
    bool add_shader(const std::string &name, const wchar_t *vertex_file, const wchar_t *pixel_file);

    void add_triangle(const vertex &a, const vertex &b, const vertex &c, const std::string &shader = "basic");

    void render_main(const camera &active_camera);
};