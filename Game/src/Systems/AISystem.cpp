#include "pch.h"
#include "AISystem.h"
#include "Scene.h"
#include "Components\AIComponent.h"
#include "ECS\Components\TransformComponent.h"
#include "ECS\Components\RigidbodyComponent.h"
#include "ECS\EntityManager.h"
#include "Events\EngineEvent.h"
#include "Events\GameEvent.h"
#include "Events\EventManager.h"

using namespace engine;
using namespace DirectX;

AISystem::AISystem() : System()
{
	RequireComponent<AIComponent>();
	RequireComponent<RigidbodyComponent>();
	RequireComponent<TransformComponent>();

	EventManager::Get().Subscribe(EventType::Collision, this);
	EventManager::Get().Subscribe(EventType::Death, this);
	EventManager::Get().Subscribe(EventType::PushBack, this);
}

bool AISystem::Init()
{
	return true;
}

void AISystem::OnUpdate(float dt)
{
	//ENGINE_TRACE("AISystem::OnUpdate thread_id {0}", std::this_thread::get_id());
	//for (auto& e : _entities)
	//{
	//	auto tc = e.GetComponent<TransformComponent>();
	//	auto rbc = e.GetComponent<RigidbodyComponent>();
	//	auto aic = e.GetComponent<AIComponent>();

	//	if (aic->target)
	//	{
	//		
	//	}
	//}
}

void AISystem::OnEvent(Event* e)
{
	ENGINE_TRACE("AISystem::OnEvent {0}", e->type);

	switch (e->type)
	{
	case EventType::Collision:
	{
		HandleCollisionEvent(e);
		break;
	}
	case EventType::Death:
	{
		HandleDeathEvent(e);
		break;
	}
	case EventType::PushBack:
	{
		HandlePushBackEvent(e);
		break;
	}
	default:
		break;
	}
}

void AISystem::HandleCollisionEvent(Event* e)
{
	auto ev = static_cast<CollisionEvent*>(e);

	// ENTITY A
	
	auto entityA = ev->entityDataA.entity;
	auto rbcA = entityA->GetComponent<RigidbodyComponent>();
	//auto currentVelA = rbcA->velocity;
	//XMStoreFloat3(&rbcA->velocity, XMVectorZero());
	
	auto aiA = entityA->GetComponent<AIComponent>();
	aiA->state = AIState::attacking;
	aiA->target = entityA;


	// ENTITY B

	auto entityB = ev->entityDataB.entity;
	auto rbcB = entityB->GetComponent<RigidbodyComponent>();
	//auto currentVelB = rbcB->velocity;
	//XMStoreFloat3(&rbcB->velocity, XMVectorZero());
	
	auto aiB = entityB->GetComponent<AIComponent>();
	aiB->state = AIState::attacking;
	aiB->target = entityB;

	// Combat

	CombatEvent cEvent(entityA, entityB);
	EventManager::Get().Trigger(&cEvent);

}

void AISystem::HandleDeathEvent(Event* e)
{
	auto ev = static_cast<DeathEvent*>(e);

	auto ai = ev->entity->GetComponent<AIComponent>();
	ai->state = AIState::dead;
	ai->target = nullptr;

	ev->entity->DestroyItSelf();
}

void AISystem::HandlePushBackEvent(Event* e)
{
	auto ev = static_cast<PushBackEvent*>(e);

	auto ai = ev->entity->GetComponent<AIComponent>();
	ai->state = AIState::idle;
	ai->target = nullptr;

	auto tc = ev->entity->GetComponent<TransformComponent>();
	auto rbc = ev->entity->GetComponent<RigidbodyComponent>();
	auto newPosB = XMLoadFloat3(&tc->position) + (-XMLoadFloat3(&rbc->velocity));
	XMStoreFloat3(&tc->position, newPosB);
}

