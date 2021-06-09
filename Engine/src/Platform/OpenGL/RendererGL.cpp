#include "pch.h"

#if _GL
#include "RendererGL.h"

#include "ECS\Components\TransformComponent.h"
#include "ECS\Components\RenderComponent.h"

using namespace engine;

bool RendererGL::Init()
{
	return true;
}

void RendererGL::ClearScreen()
{

}

void RendererGL::SwapBuffers()
{

}

void RendererGL::Render(RenderComponent* rc, TransformComponent* tc)
{

}


#endif