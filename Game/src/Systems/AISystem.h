#pragma once

#include "ECS\System.h"

class engine::Scene;

class AISystem : public engine::System
{
public:
	AISystem();

	virtual bool Init() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(engine::Event* e) override;

	void HandleCollisionEvent(engine::Event* e);
	void HandleDeathEvent(engine::Event* e);
	void HandlePushBackEvent(engine::Event* e);
};