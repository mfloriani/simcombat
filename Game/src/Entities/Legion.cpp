#include "pch.h"
#include "Legion.h"
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

void Legion::Create(engine::Scene* scene, XMFLOAT3 position, Direction direction)
{
	
	auto mesh = ResourceManager::Get().GetMesh("quad");
	auto texture = ResourceManager::Get().GetTexture("sword.dds");

	auto entity = scene->CreateEntity();
	entity.AddComponent<TransformComponent>(position);
	entity.AddComponent<RenderComponent>(mesh, texture);
	entity.AddComponent<RigidbodyComponent>(1.f, XMFLOAT3{0.3f * static_cast<int>(direction), 0.f, 0.f });
	entity.AddComponent<ColliderComponent>();
	entity.AddComponent<AIComponent>();
	entity.AddComponent<CombatComponent>(5, UnitType::Legion);
}