#pragma once
#include "3dre.h"
#include "vertex.h"
#include <vector>

class shader
{
public:
    ComPtr<ID3D11VertexShader> vs;
    ComPtr<ID3D11PixelShader> ps;
    ComPtr<ID3D11InputLayout> layout;

    std::vector<vertex> vertices;
};