#include "3dre.h"
#include "input.h"
#include "renderer.h"
#include "engine.h"

#include <chrono>

using namespace renderer;

static void CreateRenderTarget()
{
	ComPtr<ID3D11Texture2D> backBuffer;
	g_swap_chain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	g_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &g_render_target);
}

static void OnResize(UINT width, UINT height)
{
	if (!g_swap_chain || width == 0 || height == 0)
		return;

	g_context->OMSetRenderTargets(0, nullptr, nullptr);
	g_render_target.Reset(); // must release before resizing
	g_swap_chain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	CreateRenderTarget();

	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MaxDepth = 1.0f;
	g_context->RSSetViewports(1, &vp);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		OnResize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			flags::g_running = false;
		input::handle_key_down(wParam); // send to keydown handler
		return 0;
	case WM_KEYUP:
		input::handle_key_up(wParam); // send to keyup handler
		return 0;
	case WM_KILLFOCUS:
		input::reset_keys(); // avoid keys staying held if focus is lost
		return 0;
	case WM_DESTROY:
		flags::g_running = false;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

static bool InitD3D(HWND hwnd, UINT width, UINT height)
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 2;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG; // needs the Graphics Tools optional Windows feature
#endif

	D3D_FEATURE_LEVEL requested = D3D_FEATURE_LEVEL_11_0;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
		&requested, 1, D3D11_SDK_VERSION,
		&scd, &g_swap_chain, &g_device, nullptr, &g_context);

	if (FAILED(hr))
		return false;

	CreateRenderTarget();
	OnResize(width, height); // sets the initial viewport
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	const char *className = "EngineWindowClass";

	WNDCLASSEXA wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = className;
	RegisterClassExA(&wc);

	// Ask for a 1280x720 *client* area, then let Windows compute the outer size
	RECT rect = {0, 0, 1280, 720};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowExA(
		0, className, _3dre::window_title.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
		return 1;

	if (!InitD3D(hwnd, 1280, 720))
	{
		MessageBoxA(hwnd, "Failed to initialise Direct3D 11", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	flags::g_running = true;
	ShowWindow(hwnd, nCmdShow);

	// TODO: Start the tps methods using the scheduler.

	// fps vars
	auto fps_timer = std::chrono::steady_clock::now();
	int fps_frames = 0;
	int fps = 0;

	// initialize once for our engine render initializer.
	engine::render_init();

	// ---- Main loop ----
	MSG msg = {};
	while (flags::g_running)
	{
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				flags::g_running = false;
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		// fps
		fps_frames++;
		auto now = std::chrono::steady_clock::now();
		if (now - fps_timer >= std::chrono::seconds(1))
		{
			fps = fps_frames;
			fps_frames = 0;
			fps_timer = now;
			if (flags::title_fps)
				SetWindowTextA(hwnd, ("fps: " + std::to_string(fps)).c_str());
		}

		// ---- Render ----
		const float black[4] = {0, 0, 0, 1}; // black
		g_context->OMSetRenderTargets(1, g_render_target.GetAddressOf(), nullptr);
		g_context->ClearRenderTargetView(g_render_target.Get(), black);

		engine::render_frame();

		render_main(); // call render main

		g_swap_chain->Present(1, flags::VSYNC); // 1 = vsync
	}

	return 0;
}
