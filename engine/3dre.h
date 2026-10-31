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

inline void log_errors(ComPtr<ID3DBlob> &errors)
{
    if (errors)
        OutputDebugStringA(
            static_cast<const char *>(errors->GetBufferPointer()));
}

namespace _3dre
{
    inline std::string VERSION = "0.0.0.0";
};

namespace flags
{
    inline bool VSYNC = false;
    inline bool g_running = true;
    inline bool title_fps = true;

    inline int window_starting_width = 1280;
    inline int window_starting_height = 720;
    inline float default_bg_color[4] = {0, 0, 0, 1}; // black

    inline std::string window_title = "3dre";
};