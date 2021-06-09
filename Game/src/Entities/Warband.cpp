#include "pch.h"
#include "Warband.h"
#include "ResourceManager.h"
#include "Platform\Mesh.h"

#include "ECS\Components\TransformComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\RigidbodyComponent.h"
#include "ECS\Components\ColliderComponent.h"
#include "Components\AIComponent.h"
#include "Components\CombatComponent.h"

#include "Scene.h"

using namespace engine;
using namespace DirectX;

void Warband::Create(Scene* scene, XMFLOAT3 position, Direction direction)
{
	auto mesh = ResourceManager::Get().GetMesh("quad");
	auto texture = ResourceManager::Get().GetTexture("axe.dds");

	auto entity = scene->CreateEntity();
	entity.AddComponent<TransformComponent>(position);
	entity.AddComponent<RenderComponent>(mesh, texture);
	entity.AddComponent<RigidbodyComponent>(1.f, XMFLOAT3{ 0.9f * static_cast<int>(direction), 0.f, 0.f });
	entity.AddComponent<ColliderComponent>();
	entity.AddComponent<AIComponent>();
	entity.AddComponent<CombatComponent>(3, UnitType::Warband);
}
