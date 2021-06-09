#include "pch.h"
#include "CollisionSystem.h"
#include "ECS\Components\TransformComponent.h"
#include "ECS\Components\ColliderComponent.h"
#include "ECS\Components\RigidbodyComponent.h"
#include "ECS\EntityManager.h"
#include "Events\EngineEvent.h"
#include "Events\EventManager.h"

using namespace engine;
using namespace DirectX;

CollisionSystem::CollisionSystem() : System()
{
	RequireComponent<TransformComponent>();
	RequireComponent<ColliderComponent>();
	RequireComponent<RigidbodyComponent>();
	
}

CollisionSystem::~CollisionSystem()
{
	
}

bool CollisionSystem::Init()
{
	

	return true;
}

void CollisionSystem::OnUpdate(float dt)
{
	//ENGINE_TRACE("CollisionSystem::OnUpdate {0}", dt);
	if (_entities.size() == 0)
		return;

	for (int i = 0; i < _entities.size()-1; ++i)
	{
		for (int j = i+1; j < _entities.size(); ++j)
		{
			auto& entityA = _entities[i];
			auto& entityB = _entities[j];

			if (HasCollision(entityA, entityB))
			{
				CollisionData a;
				a.entity = &entityA;
				a.velocity = entityA.GetComponent<RigidbodyComponent>()->velocity;

				CollisionData b;
				b.entity = &entityB;
				b.velocity = entityB.GetComponent<RigidbodyComponent>()->velocity;

				CollisionEvent e(a, b);
				EventManager::Get().Trigger(&e);
			}
		}
	}
}

bool CollisionSystem::HasCollision(Entity& a, Entity& b)
{
	auto tA = a.GetComponent<TransformComponent>();
	auto tB = b.GetComponent<TransformComponent>();

	auto collA = a.GetComponent<ColliderComponent>();
	auto collB = b.GetComponent<ColliderComponent>();

	float radiusSum = collA->radius + collB->radius;
	float sqRadius = radiusSum * radiusSum;
	
	XMFLOAT3 sqDist;
	XMStoreFloat3(&sqDist, XMVector3LengthSq( XMLoadFloat3(&tB->position) - XMLoadFloat3(&tA->position) ));

	//ENGINE_TRACE("sqDist {0} < {1}", sqDist.x, sqRadius);

	return sqDist.x < sqRadius;
}

void CollisionSystem::OnEvent(Event* e)
{

}
