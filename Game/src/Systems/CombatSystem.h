#pragma once

#include "ECS\System.h"

struct engine::Event;
class engine::Scene;

class CombatSystem : public engine::System
{
public:
	CombatSystem();
	virtual ~CombatSystem();

	virtual bool Init() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(engine::Event* e) override;

	void HandleCombatEvent(engine::Event* e);
};
