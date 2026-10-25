#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>

#include "3dre.h"
#include "input.h"

using Microsoft::WRL::ComPtr;

// ---- Globals (move into an Engine/Renderer class once this grows) ----
static ComPtr<ID3D11Device> g_device;
static ComPtr<ID3D11DeviceContext> g_context;
static ComPtr<IDXGISwapChain> g_swapChain;
static ComPtr<ID3D11RenderTargetView> g_rtv;
static bool g_running = true;

static void CreateRenderTarget()
{
	ComPtr<ID3D11Texture2D> backBuffer;
	g_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	g_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &g_rtv);
}

static void OnResize(UINT width, UINT height)
{
	if (!g_swapChain || width == 0 || height == 0)
		return;

	g_context->OMSetRenderTargets(0, nullptr, nullptr);
	g_rtv.Reset(); // must release before resizing
	g_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
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
			g_running = false;
		input::handle_key_down(wParam); // send to keydown handler
		return 0;
	case WM_KEYUP:
		input::handle_key_up(wParam); // send to keyup handler
		return 0;
	case WM_KILLFOCUS:
		input::reset_keys(); // avoid keys staying held if focus is lost
		return 0;
	case WM_DESTROY:
		g_running = false;
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
		&scd, &g_swapChain, &g_device, nullptr, &g_context);

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
		0, className, "3dre", WS_OVERLAPPEDWINDOW,
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

	ShowWindow(hwnd, nCmdShow);

	// ---- Main loop ----
	MSG msg = {};
	while (g_running)
	{
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				g_running = false;
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		// ---- Render ----
		const float black[4] = {0, 0, 0, 1}; // black
		g_context->OMSetRenderTargets(1, g_rtv.GetAddressOf(), nullptr);
		g_context->ClearRenderTargetView(g_rtv.Get(), black);

		// TODO: your engine's drawing goes here

		if(KEY_A_PRESSED) std::cout << "'A' was pressed." << std::endl;

		g_swapChain->Present(1, 0); // 1 = vsync
	}

	return 0;
}
