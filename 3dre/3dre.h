#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <iostream>
#include <string>

using Microsoft::WRL::ComPtr;

namespace _3dre
{
    inline std::string VERSION = "0.0.0.0";
    inline std::string window_title = "3dre";
};

namespace flags
{
    inline bool VSYNC = false;
    inline bool g_running = true;
    inline bool title_fps = true;
};