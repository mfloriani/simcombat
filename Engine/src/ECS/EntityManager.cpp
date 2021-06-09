#include "pch.h"
#include "EntityManager.h"
#include "Engine.h"

using namespace engine;

void EntityManager::AddToSystems(EntityID id)
{
	ECSSignature signature = _entitySignatureMap[id];
	auto ev = std::make_shared<EntityCreatedEvent>(GetEntity(id), signature);
	Engine::Get().BroadcastEvent(ev);
}

void EntityManager::RemoveFromSystems(EntityID id)
{
	auto ev = std::make_shared<EntityDestroyedEvent>(GetEntity(id));
	Engine::Get().BroadcastEvent(ev);
}
