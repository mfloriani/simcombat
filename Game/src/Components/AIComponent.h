#pragma once

#include "Constants.h"
#include "ECS\Component.h"

enum class AIState
{
	idle,
	moving,
	attacking,
	dead
};

struct AIComponent : public engine::Component
{
	AIComponent() : target(nullptr), state(AIState::idle)
	{

	}
	
	engine::Entity* target;
	AIState state;
};