#include "pch.h"
#include "Engine.h"

#include "windowsx.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_impl_dx11.h"

using namespace engine;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#if _DX
	// TODO: it should be Window* instead of WindowDX (need pure virtual functions)
	WindowDX* const pWnd = reinterpret_cast<WindowDX*>(GetWindowLongPtrW(hwnd, 0));
#elif _GL
	WindowGL* const pWnd = reinterpret_cast<WindowGL*>(GetWindowLongPtrW(hwnd, 0));
#endif
	if (!pWnd)
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_CLOSE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		pWnd->GetOwner()->OnKeyDown(wParam);
		break;

	case WM_KEYUP:
		pWnd->GetOwner()->OnKeyUp(wParam);
		break;

	case WM_MOUSEMOVE:
		{
		//auto mousePos = MOUSEMOVEPOINT();
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		//ENGINE_TRACE("WM_MOUSEMOVE {0},{1}", xPos, yPos);
		// TODO: mousemove event

		}
		break;

	//case WM_SIZE:
		//_renderer->CreateWindowSizeResources();
		//break;

	case WM_EXITSIZEMOVE:
		//_renderer->OnResize();
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Engine::OnKeyDown(WPARAM key)
{
	auto e = std::make_shared<KeyDownEvent>(key);
	_mainThread_EventQ.AddItem(e);
	_updateThread_EventQ.AddItem(e);
	_renderThread_EventQ.AddItem(e);
	_networkThread_EventQ.AddItem(e);
}

void Engine::OnKeyUp(WPARAM key)
{
	auto e = std::make_shared<KeyUpEvent>(key);
	_mainThread_EventQ.AddItem(e);
	_updateThread_EventQ.AddItem(e);
	_renderThread_EventQ.AddItem(e);
	_networkThread_EventQ.AddItem(e);
}

void Engine::BroadcastEvent(std::shared_ptr<Event> e)
{
	_mainThread_EventQ.AddItem(e);
	_updateThread_EventQ.AddItem(e);
	_renderThread_EventQ.AddItem(e);
	_networkThread_EventQ.AddItem(e);
}