#include "pch.h"
#include "RenderingSystem.h"
#include "ECS\Components\TransformComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\EntityManager.h"
#include "Events\Event.h"
#include "Events\EventManager.h"

using namespace engine;

RenderingSystem::RenderingSystem(Renderer* renderer) : System(), _renderer(renderer)
{
    RequireComponent<TransformComponent>();
    RequireComponent<RenderComponent>();

    //ENGINE_TRACE("RenderingSystem signature: {0}", GetSignature());

}

bool RenderingSystem::Init()
{
    
    return true;
}

void RenderingSystem::OnUpdate(float dt)
{
    //ENGINE_TRACE("dt {0}", dt);
    for (auto& e : _entities)
    {
        auto tc = e.GetComponent<TransformComponent>();
        auto rc = e.GetComponent<RenderComponent>();

        _renderer->Render(rc, tc);
    }
}

void RenderingSystem::OnEvent(Event* e)
{
}
