#pragma once

#include "ECS\System.h"

namespace engine
{
	struct Event;
	class Scene;

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		virtual ~PhysicsSystem();

		virtual bool Init() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnEvent(Event* e) override;

	};
}