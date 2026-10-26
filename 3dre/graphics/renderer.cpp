#include "renderer.h"

namespace renderer
{
    static ComPtr<ID3D11Device> g_device;
    static ComPtr<ID3D11DeviceContext> g_context;
    static ComPtr<IDXGISwapChain> g_swapChain;
    static ComPtr<ID3D11RenderTargetView> g_rtv;

    void render_main()
    {
    }
};