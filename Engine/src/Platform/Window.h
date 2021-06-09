#pragma once

#include <Windows.h>

namespace engine
{
	class IWindowOwner
	{
	public:
		virtual void OnKeyDown(WPARAM key)=0;
		virtual void OnKeyUp(WPARAM key)=0;
	};

	class Window
	{
	public:
		Window(IWindowOwner* owner);
		virtual ~Window() = default;

		virtual bool Init(HINSTANCE hInstance, WNDPROC wndProc) = 0;

		IWindowOwner* GetOwner() { return _owner; }

	protected:
		IWindowOwner* _owner;
	};
}