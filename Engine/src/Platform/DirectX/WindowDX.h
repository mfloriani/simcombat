#pragma once

#if _DX

#include <memory>

#include "Platform\Window.h"

namespace engine
{
	class WindowDX : public Window
	{
	public:
		WindowDX(IWindowOwner* owner);
		virtual bool Init(HINSTANCE hInstance, WNDPROC wndProc) override;
		HWND GetHwnd() const { return _hWnd; }
	private:
		HWND _hWnd;
	};
}

#endif