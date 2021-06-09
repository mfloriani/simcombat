#include "pch.h"
#include "SystemManager.h"

using namespace engine;

SystemManager::SystemManager()
{
}

SystemManager::~SystemManager()
{

}

void SystemManager::EntityCreated(Entity e, ECSSignature eSignature)
{
	for (auto& sys : _systems)
	{
		auto& systemSignature = sys->GetSignature();
		if ((systemSignature & eSignature) == systemSignature)
		{
			sys->AddEntity(e);
		}
	}
}

void SystemManager::EntityDestroyed(Entity e)
{
	for (auto& sys : _systems)
	{
		sys->RemoveEntity(e);
	}
}

void SystemManager::OnUpdate(float dt)
{
	for (auto& sys : _systems)
	{
		sys->OnUpdate(dt);
	}
}
