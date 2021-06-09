#pragma once

#if _GL

#include <memory>

#include "Platform\Window.h"
//#include "RendererGL.h"

//#include "GLAD\glad.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

namespace engine
{
	class IWindowOwner;

	class WindowGL : public Window
	{
	public:
		WindowGL(IWindowOwner* owner);
		virtual bool Init(HINSTANCE hInstance, WNDPROC wndProc) override;
		
		GLFWwindow* GetWindow() { return _window; }

	private:
		GLFWwindow* _window;
		//std::unique_ptr<RendererDX> _renderer;
	};
}

#endif