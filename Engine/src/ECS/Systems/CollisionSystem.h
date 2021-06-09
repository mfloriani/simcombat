#pragma once

#include "ECS\System.h"

namespace engine
{
	struct Event;
	
	class CollisionSystem : public System
	{
	public:
		CollisionSystem();
		virtual ~CollisionSystem();

		virtual bool Init() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnEvent(Event* e) override;

		bool HasCollision(Entity& a, Entity& b);
	};
}