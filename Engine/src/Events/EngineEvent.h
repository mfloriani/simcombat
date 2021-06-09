#pragma once

#include "Events\Event.h"
#include "ECS\Entity.h"
#include "Constants.h"

#include <DirectXMath.h>
#include <bitset>
#include <string>
#include <memory>

namespace engine
{
	class Entity;

	struct StartGameEvent : public Event
	{
		StartGameEvent() : 
			Event(EventType::StartGame)			
		{}
	};

	struct QuitGameEvent : public Event
	{
		QuitGameEvent() : Event(EventType::QuitGame) {}
	};

	struct PeerReadyEvent : public Event
	{
		PeerReadyEvent(const std::string& peerName, UnitType type, int qty) :
			Event(EventType::PeerReady),
			peerName(peerName),
			unitType(type),
			unitQty(qty)
		{}

		std::string peerName;
		UnitType unitType;
		int unitQty;
	};

	struct JoinGameEvent : public Event
	{
		JoinGameEvent() : Event(EventType::JoinGame) {}
	};

	struct HostGameEvent : public Event
	{
		HostGameEvent() : Event(EventType::HostGame) {}
	};

	struct CollisionData
	{
		Entity* entity;
		DirectX::XMFLOAT3 velocity;
	};

	struct CollisionEvent : public Event
	{
		CollisionEvent(CollisionData& a, CollisionData& b)
			: Event(EventType::Collision), entityDataA(a), entityDataB(b)
		{}

		CollisionData entityDataA;
		CollisionData entityDataB;
	};

	struct CombatEvent : public Event
	{
		CombatEvent(Entity* a, Entity* b)
			: Event(EventType::Combat), entityA(a), entityB(b)
		{}

		Entity* entityA;
		Entity* entityB;
	};

	struct DeathEvent : public Event
	{
		DeathEvent(Entity* e)
			: Event(EventType::Death), entity(e)
		{}

		Entity* entity;
	};

	struct EntityCreatedEvent : public Event
	{
		EntityCreatedEvent(Entity& e, ECSSignature signature) :
			Event(EventType::EntityCreated), 
			entity(e),
			entitySignature(signature)
		{}

		Entity       entity;
		ECSSignature entitySignature;
	};

	struct EntityDestroyedEvent : public Event
	{
		EntityDestroyedEvent(Entity& e) :
			Event(EventType::EntityDestroyed), 
			entity(e)
		{}

		Entity entity;
	};

	struct LoadSceneEvent : public Event
	{
		LoadSceneEvent(const std::string& sceneId) :
			Event(EventType::LoadScene),
			sceneId(sceneId)
		{}
		std::string sceneId;
	};
	
	struct CPUAffinityEvent : public Event
	{
		CPUAffinityEvent(int cpu) :
			Event(EventType::CPUAffinity),
			cpu(cpu)
		{}
		int cpu;
	};
}