#include "3dre.h"
#include "input.h"
#include "renderer.h"
#include "engine.h"

#include <chrono>

using namespace renderer;

static bool CreateRenderTarget()
{
	ComPtr<ID3D11Texture2D> back_buffer;

	HRESULT hr = g_swap_chain->GetBuffer(
		0, IID_PPV_ARGS(&back_buffer));

	if (FAILED(hr))
		return false;

	hr = g_device->CreateRenderTargetView(
		back_buffer.Get(),
		nullptr,
		&g_render_target);

	if (FAILED(hr))
		return false;

	return true;
}

static void SetViewport(UINT width, UINT height)
{
	if (width == 0 || height == 0)
		return;

	D3D11_VIEWPORT viewport{};

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	g_context->RSSetViewports(1, &viewport);
}

static void OnResize(UINT width, UINT height)
{
	if (!g_swap_chain || width == 0 || height == 0)
		return;

	// Release references to the old back buffer.
	g_context->OMSetRenderTargets(0, nullptr, nullptr);
	g_render_target.Reset();

	HRESULT hr = g_swap_chain->ResizeBuffers(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0);

	if (FAILED(hr))
	{
		OutputDebugStringA("Failed to resize swap chain.\n");
		return;
	}

	if (!CreateRenderTarget())
	{
		OutputDebugStringA("Failed to recreate render target.\n");
		return;
	}

	SetViewport(width, height);

	main_camera.set_aspect_ratio(
		static_cast<float>(width),
		static_cast<float>(height));
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
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL requested = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
		&requested, 1, D3D11_SDK_VERSION,
		&scd, &g_swap_chain, &g_device, nullptr, &g_context);

	if (FAILED(hr))
		return false;

	if (!CreateRenderTarget())
		return false;

	SetViewport(width, height);

	main_camera.set_aspect_ratio(
		static_cast<float>(width),
		static_cast<float>(height));

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
	RECT rect = {0, 0, flags::window_starting_width, flags::window_starting_height};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowExA(
		0, className, flags::window_title.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
		return 1;

	if (!InitD3D(hwnd, flags::window_starting_width, flags::window_starting_height))
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
		if (g_render_target)
		{
			// Bind the back buffer for this frame.
			g_context->OMSetRenderTargets(1, g_render_target.GetAddressOf(), nullptr);

			// Clear the previous frame.
			g_context->ClearRenderTargetView(g_render_target.Get(), flags::default_bg_color);

			// Render the scene.
			engine::render_frame(); // move this to renderer.cpp
			render_main(main_camera);

			// Display the completed frame.
			g_swap_chain->Present(1, flags::VSYNC);
		}
	}
	return 0;
}
