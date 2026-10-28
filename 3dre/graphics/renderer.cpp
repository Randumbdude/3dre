#include "renderer.h"

#include "vertex.h"

#include <cstddef>
#include <iterator>
#include <d3dcompiler.h>

namespace renderer
{
    ComPtr<ID3D11Device> g_device;
    ComPtr<ID3D11DeviceContext> g_context;
    ComPtr<IDXGISwapChain> g_swap_chain;
    ComPtr<ID3D11RenderTargetView> g_render_target;

    namespace
    {
        ComPtr<ID3D11Buffer> triangle_vertex_buffer;
        ComPtr<ID3D11VertexShader> triangle_vertex_shader;
        ComPtr<ID3D11PixelShader> triangle_pixel_shader;
        ComPtr<ID3D11InputLayout> triangle_input_layout;
        bool triangle_pipeline_attempted = false;

        bool create_triangle_pipeline()
        {
            if (!g_device)
                return false;

            // Positions are in clip space, so the triangle is visible without a camera.
            const vertex vertices[] = {
                vertex(-0.65f, -0.55f, 0.0f),
                vertex(0.0f, 0.65f, 0.0f),
                vertex(0.65f, -0.55f, 0.0f)};

            D3D11_BUFFER_DESC buffer_description = {};
            buffer_description.ByteWidth = sizeof(vertices);
            buffer_description.Usage = D3D11_USAGE_DEFAULT;
            buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            D3D11_SUBRESOURCE_DATA initial_data = {};
            initial_data.pSysMem = vertices;

            HRESULT hr = g_device->CreateBuffer(
                &buffer_description, &initial_data, &triangle_vertex_buffer);
            if (FAILED(hr))
            {
                OutputDebugStringA("Failed to create triangle vertex buffer.\n");
                return false;
            }

            static constexpr char shader_source[] = R"(
                struct VertexInput
                {
                    float3 position : POSITION;
                };

                struct VertexOutput
                {
                    float4 position : SV_POSITION;
                };

                VertexOutput VSMain(VertexInput input)
                {
                    VertexOutput output;
                    output.position = float4(input.position, 1.0f);
                    return output;
                }

                float4 PSMain() : SV_TARGET
                {
                    return float4(0.15f, 0.75f, 0.95f, 1.0f);
                }
            )";

            ComPtr<ID3DBlob> vertex_bytecode;
            ComPtr<ID3DBlob> errors;
            hr = D3DCompile(
                shader_source, sizeof(shader_source) - 1, nullptr, nullptr, nullptr,
                "VSMain", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0,
                &vertex_bytecode, &errors);
            if (FAILED(hr))
            {
                if (errors)
                    OutputDebugStringA(static_cast<const char *>(errors->GetBufferPointer()));
                return false;
            }

            hr = g_device->CreateVertexShader(
                vertex_bytecode->GetBufferPointer(), vertex_bytecode->GetBufferSize(),
                nullptr, &triangle_vertex_shader);
            if (FAILED(hr))
            {
                OutputDebugStringA("Failed to create triangle vertex shader.\n");
                return false;
            }

            const D3D11_INPUT_ELEMENT_DESC input_elements[] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                 static_cast<UINT>(offsetof(vertex, pos)), D3D11_INPUT_PER_VERTEX_DATA, 0}};

            hr = g_device->CreateInputLayout(
                input_elements, static_cast<UINT>(std::size(input_elements)),
                vertex_bytecode->GetBufferPointer(), vertex_bytecode->GetBufferSize(),
                &triangle_input_layout);
            if (FAILED(hr))
            {
                OutputDebugStringA("Failed to create triangle input layout.\n");
                return false;
            }

            ComPtr<ID3DBlob> pixel_bytecode;
            errors.Reset();
            hr = D3DCompile(
                shader_source, sizeof(shader_source) - 1, nullptr, nullptr, nullptr,
                "PSMain", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0,
                &pixel_bytecode, &errors);
            if (FAILED(hr))
            {
                if (errors)
                    OutputDebugStringA(static_cast<const char *>(errors->GetBufferPointer()));
                return false;
            }

            hr = g_device->CreatePixelShader(
                pixel_bytecode->GetBufferPointer(), pixel_bytecode->GetBufferSize(),
                nullptr, &triangle_pixel_shader);
            if (FAILED(hr))
            {
                OutputDebugStringA("Failed to create triangle pixel shader.\n");
                return false;
            }

            return true;
        }
    }

    void render_main()
    {
        if (!g_context || !g_device)
            return;

        if (!triangle_vertex_buffer && !triangle_pipeline_attempted)
        {
            triangle_pipeline_attempted = true;
            if (!create_triangle_pipeline())
                return;
        }

        if (!triangle_vertex_buffer || !triangle_vertex_shader ||
            !triangle_pixel_shader || !triangle_input_layout)
            return;

        constexpr UINT stride = sizeof(vertex);
        constexpr UINT offset = 0;
        ID3D11Buffer *vertex_buffer = triangle_vertex_buffer.Get();

        g_context->IASetInputLayout(triangle_input_layout.Get());
        g_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
        g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        g_context->VSSetShader(triangle_vertex_shader.Get(), nullptr, 0);
        g_context->PSSetShader(triangle_pixel_shader.Get(), nullptr, 0);
        g_context->Draw(3, 0);
    }
}
