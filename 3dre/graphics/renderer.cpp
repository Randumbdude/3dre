#include "renderer.h"

namespace renderer
{
    ComPtr<ID3D11Device> g_device;
    ComPtr<ID3D11DeviceContext> g_context;
    ComPtr<IDXGISwapChain> g_swap_chain;
    ComPtr<ID3D11RenderTargetView> g_render_target;

    void render_main()
    {
    }
};