#pragma once

#include "ECS\System.h"
#include "Platform\Renderer.h"

#include <vector>
#include <memory>

namespace engine
{
	struct Event;

	class RenderingSystem : public System
	{
	public:
		RenderingSystem(Renderer* renderer);

	public:
		virtual bool Init() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnEvent(Event* e) override;

		//void SetRenderer(Renderer* renderer) { _renderer = renderer; }

	private:
		Renderer* _renderer;
		//std::vector<Component> _components;
	};
}