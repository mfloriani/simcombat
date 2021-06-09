#pragma once

#if _GL

#include "Platform\Renderer.h"

namespace engine
{
	struct RenderComponent;
	struct TransformComponent;

	class RendererGL : public Renderer
	{
	public:
		virtual bool Init() override;
		virtual void ClearScreen() override;
		virtual void SwapBuffers() override;
		virtual void Render(RenderComponent* rc, TransformComponent* tc) override;
	};
}

#endif