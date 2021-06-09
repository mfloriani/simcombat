#include "pch.h"
#include "Constants.h"

#if _GL

#include "WindowGL.h"


using namespace engine;

WindowGL::WindowGL(IWindowOwner* owner) : Window(owner)
{
}

bool WindowGL::Init(HINSTANCE hInstance, WNDPROC wndProc)
{
    if (!glfwInit())
    {
        ENGINE_ERROR("Error initializing GLFW!");
        return false;
    }

    _window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SimCombat", NULL, NULL);
    if (!_window)
    {
        ENGINE_ERROR("Error creating GLFW window!");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(_window);

    if (glewInit() != GLEW_OK)
    {
        ENGINE_ERROR("Error initializing GLEW!");
        glfwTerminate();
        return false;
    }

    ENGINE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
    
    return true;
}


#endif
