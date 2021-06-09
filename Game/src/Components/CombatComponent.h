#pragma once

#include "Constants.h"
#include "ECS\Component.h"

struct CombatComponent : public engine::Component
{
	CombatComponent(int attack, engine::UnitType type) : attack(attack), unitType(type)
	{

	}

	int attack;
	engine::UnitType unitType;
};