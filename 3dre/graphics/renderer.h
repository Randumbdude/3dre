#pragma once
#include "3dre.h"

namespace renderer
{
    extern ComPtr<ID3D11Device> g_device;
    extern ComPtr<ID3D11DeviceContext> g_context;
    extern ComPtr<IDXGISwapChain> g_swap_chain;
    extern ComPtr<ID3D11RenderTargetView> g_render_target;

    void render_main();
};