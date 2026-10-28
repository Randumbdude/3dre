#include "renderer.h"
#include "vertex.h"
#include "3dre.h"

#include <d3dcompiler.h>
#include <wrl/client.h>

namespace renderer
{
    ComPtr<ID3D11Device> g_device;
    ComPtr<ID3D11DeviceContext> g_context;
    ComPtr<IDXGISwapChain> g_swap_chain;
    ComPtr<ID3D11RenderTargetView> g_render_target;

    void log_errors(ComPtr<ID3DBlob> &errors)
    {
        if (errors)
            OutputDebugStringA(
                static_cast<const char *>(errors->GetBufferPointer()));
    }

    bool add_shader(const std::string &name, const wchar_t *vertex_file, const wchar_t *pixel_file)
    {
        if (!g_device)
            return false;

        shader shader;
        ComPtr<ID3DBlob> vs_blob, ps_blob, errors;

        // Compile vertex shader from file.
        HRESULT hr = D3DCompileFromFile(
            vertex_file, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "VSMain", "vs_4_0",
            D3DCOMPILE_ENABLE_STRICTNESS, 0,
            &vs_blob, &errors);

        if (FAILED(hr))
        {
            log_errors(errors);
            return false;
        }

        // Compile pixel shader from file.
        errors.Reset();

        hr = D3DCompileFromFile(
            pixel_file, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "PSMain", "ps_4_0",
            D3DCOMPILE_ENABLE_STRICTNESS, 0,
            &ps_blob, &errors);

        if (FAILED(hr))
        {
            log_errors(errors);
            return false;
        }

        // Create GPU shader objects.
        hr = g_device->CreateVertexShader(
            vs_blob->GetBufferPointer(),
            vs_blob->GetBufferSize(),
            nullptr, &shader.vs);

        if (FAILED(hr))
            return false;

        hr = g_device->CreatePixelShader(
            ps_blob->GetBufferPointer(),
            ps_blob->GetBufferSize(),
            nullptr, &shader.ps);

        if (FAILED(hr))
            return false;

        // Describe our Vertex structure to Direct3D.
        const D3D11_INPUT_ELEMENT_DESC elements[] =
            {
                {"POSITION", 0,
                 DXGI_FORMAT_R32G32B32_FLOAT,
                 0, 0,
                 D3D11_INPUT_PER_VERTEX_DATA, 0}};

        hr = g_device->CreateInputLayout(
            elements, 1,
            vs_blob->GetBufferPointer(),
            vs_blob->GetBufferSize(),
            &shader.layout);

        if (FAILED(hr))
            return false;

        // Store it under its name, e.g. "basic".
        shaders.insert_or_assign(name, std::move(shader));
        return true;
    }

    bool add_triangle(const float p1[3], const float p2[3], const float p3[3], std::string &shader_name)
    {
        auto it = shaders.find(shader_name);

        if (it == shaders.end())
            return false;

        auto &vertices = it->second.vertices;

        vertices.push_back({p1[0], p1[1], p1[2]});
        vertices.push_back({p2[0], p2[1], p2[2]});
        vertices.push_back({p3[0], p3[1], p3[2]});

        return true;
    }

    void render_main(void)
    {
        if (!g_context || !g_device)
            return;
    }
}
