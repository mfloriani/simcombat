#pragma once

#include "ECS\System.h"

namespace engine
{
	class NetworkingSystem : public System
	{
	public:
		NetworkingSystem();
		~NetworkingSystem();

		virtual bool Init() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnEvent(Event* e) override;

	private:
		bool _isRunning;
	};
}