#include "pch.h"
#include "PhysicsSystem.h"

#include "ECS\Components\TransformComponent.h"
#include "ECS\Components\RigidbodyComponent.h"
#include "ECS\EntityManager.h"
#include "Events\Event.h"

using namespace engine;
using namespace DirectX;

PhysicsSystem::PhysicsSystem() : System()
{
    RequireComponent<TransformComponent>();
    RequireComponent<RigidbodyComponent>();

}

PhysicsSystem::~PhysicsSystem()
{
    
}

bool PhysicsSystem::Init()
{
    return true;
}

void PhysicsSystem::OnUpdate(float dt)
{
    //ENGINE_TRACE("PhysicsSystem::OnUpdate {0}", dt);

    for (auto& e : _entities)
    {
        auto tc = e.GetComponent<TransformComponent>();
        auto rbc = e.GetComponent<RigidbodyComponent>();

        XMVECTOR pos = XMLoadFloat3(&tc->position);
        XMVECTOR vel = XMLoadFloat3(&rbc->velocity);

        //float invMass = (rbc->mass > 0.0f) ? 1.0f / rbc->mass : 0.f;

        vel *= dt;

        //XMFLOAT3 v;
        //XMStoreFloat3(&v, vel);
        //ENGINE_TRACE("vel x:{0} y:{1}", v.x, v.y);

        pos += vel;

        XMStoreFloat3(&tc->position, pos);
    }
}

void PhysicsSystem::OnEvent(Event* e)
{
    ENGINE_TRACE("PhysicsSystem::OnEvent {0}", e->type);
}
