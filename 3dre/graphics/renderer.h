#pragma once
#include "3dre.h"

namespace renderer
{
    extern ComPtr<ID3D11Device> g_device;
    extern ComPtr<ID3D11DeviceContext> g_context;
    extern ComPtr<IDXGISwapChain> g_swapChain;
    extern ComPtr<ID3D11RenderTargetView> g_rtv;

    void render_main();
};