#pragma once

namespace engine
{
	struct RenderComponent;
	struct TransformComponent;

	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

	public:
		virtual bool Init() = 0;
		virtual void ClearScreen() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Render(RenderComponent* rc, TransformComponent* tc) = 0;
	};
}