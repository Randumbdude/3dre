#include "renderer.h"
#include "vertex.h"
#include "3dre.h"

#include <d3dcompiler.h>
#include <wrl/client.h>
#include <filesystem>

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

        // Load a precompiled .cso or compile a .hlsl file.
        auto load_shader = [&](const wchar_t *file,
                               const char *entry_point,
                               const char *profile,
                               ComPtr<ID3DBlob> &blob) -> bool
        {
            const std::filesystem::path path(file);
            const std::wstring extension = path.extension().wstring();

            HRESULT hr;

            if (extension == L".cso")
            {
                // Load precompiled shader bytecode.
                hr = D3DReadFileToBlob(file, &blob);

                if (FAILED(hr))
                {
                    OutputDebugStringW(
                        (L"Failed to load shader: " + path.wstring() + L"\n").c_str());
                    return false;
                }
            }
            else if (extension == L".hlsl")
            {
                // Compile HLSL source.
                hr = D3DCompileFromFile(
                    file,
                    nullptr,
                    D3D_COMPILE_STANDARD_FILE_INCLUDE,
                    entry_point,
                    profile,
                    D3DCOMPILE_ENABLE_STRICTNESS,
                    0,
                    &blob,
                    &errors);

                if (FAILED(hr))
                {
                    log_errors(errors);
                    return false;
                }
            }
            else
            {
                OutputDebugStringW(
                    (L"Unsupported shader extension: " + path.wstring() + L"\n").c_str());
                return false;
            }

            return true;
        };

        // Load or compile both shaders.
        if (!load_shader(vertex_file, "VSMain", "vs_4_0", vs_blob))
            return false;

        if (!load_shader(pixel_file, "PSMain", "ps_4_0", ps_blob))
            return false;

        // Create GPU shader objects.
        HRESULT hr = g_device->CreateVertexShader(
            vs_blob->GetBufferPointer(),
            vs_blob->GetBufferSize(),
            nullptr,
            &shader.vs);

        if (FAILED(hr))
            return false;

        hr = g_device->CreatePixelShader(
            ps_blob->GetBufferPointer(),
            ps_blob->GetBufferSize(),
            nullptr,
            &shader.ps);

        if (FAILED(hr))
            return false;

        // Describe the vertex position to Direct3D.
        const D3D11_INPUT_ELEMENT_DESC elements[] =
            {
                {"POSITION", 0,
                 DXGI_FORMAT_R32G32B32_FLOAT,
                 0, 0,
                 D3D11_INPUT_PER_VERTEX_DATA, 0}};

        // Use the vertex shader bytecode to create the input layout.
        hr = g_device->CreateInputLayout(
            elements,
            1,
            vs_blob->GetBufferPointer(),
            vs_blob->GetBufferSize(),
            &shader.layout);

        if (FAILED(hr))
            return false;

        // Store the shader under its name.
        shaders.insert_or_assign(name, std::move(shader));

        return true;
    }

    void add_triangle(const vertex &a, const vertex &b, const vertex &c, const std::string &shader_name)
    {
        auto it = shaders.find(shader_name);

        if (it == shaders.end())
            return;

        auto &vertices = it->second.vertices;

        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
    }

    void render_main(void)
    {
        if (!g_context || !g_device)
            return;

        for (auto &[name, shader] : shaders)
        {
            if (shader.vertices.empty())
                continue;

            // Upload all triangles using this shader in one buffer.
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = static_cast<UINT>(
                shader.vertices.size() * sizeof(vertex));
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            D3D11_SUBRESOURCE_DATA data{};
            data.pSysMem = shader.vertices.data();

            ComPtr<ID3D11Buffer> buffer;

            HRESULT hr = g_device->CreateBuffer(
                &desc, &data, &buffer);

            if (FAILED(hr))
            {
                shader.vertices.clear();
                continue;
            }

            constexpr UINT stride = sizeof(vertex);
            constexpr UINT offset = 0;

            ID3D11Buffer *raw_buffer = buffer.Get();

            g_context->IASetInputLayout(shader.layout.Get());
            g_context->IASetVertexBuffers(
                0, 1, &raw_buffer, &stride, &offset);
            g_context->IASetPrimitiveTopology(
                D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            g_context->VSSetShader(shader.vs.Get(), nullptr, 0);
            g_context->PSSetShader(shader.ps.Get(), nullptr, 0);

            g_context->Draw(
                static_cast<UINT>(shader.vertices.size()), 0);

            // Empty the draw list for the next frame.
            shader.vertices.clear();
        }
    }
}
