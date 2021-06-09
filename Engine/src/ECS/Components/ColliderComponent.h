#pragma once

#include "ECS\Component.h"

namespace engine
{
	struct ColliderComponent : public Component
	{
		ColliderComponent() : radius(0.05f) {}
		float radius;
	};
}