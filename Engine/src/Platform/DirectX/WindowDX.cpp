#include "pch.h"

#if _DX

#include "WindowDX.h"
#include "Constants.h"

using namespace engine;

WindowDX::WindowDX(IWindowOwner* owner) : Window(owner)
{
}

bool WindowDX::Init(HINSTANCE hInstance, WNDPROC wndProc)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = wndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(WindowDX); // used for the pointer of this object passed to the static callback
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	int centerScreenX = static_cast<int>((GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) * 0.5);
	int centerScreenY = static_cast<int>((GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) * 0.5);

	_hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"SimCombat",
		WS_OVERLAPPEDWINDOW,
		centerScreenX,
		centerScreenY,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (_hWnd == NULL)
	{
		ENGINE_ERROR("Error creating DirectX window!");
		return false;
	}

	// passing `this` pointer to the window, so I can call member function in wnd callback
	SetWindowLongPtrW(_hWnd, 0, reinterpret_cast<LONG_PTR>(this));

	if (!IsWindowVisible(_hWnd))
		ShowWindow(_hWnd, SW_SHOW);

	return true;
}

#endif