#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ECS\System.h"
#include "Events\EngineEvent.h"

using namespace engine;

Scene::Scene() : _entityManager(nullptr)
{
	
}

Scene::~Scene()
{
	//ENGINE_TRACE("Scene destructor");
}

Entity Scene::CreateEntity()
{
	return _entityManager->CreateEntity();
}

void Scene::DestroyEntity(Entity* e)
{
	_entityManager->DestroyEntity(e->GetId());
}


void Scene::Init()
{
	//GAME_TRACE("Scene::Init");
	_entityManager = std::make_unique<EntityManager>();
}

void Scene::LoadScene(const std::string& sceneId)
{
	SceneManager::Get().PushScene(sceneId);
}

void Scene::QuitScene()
{
	SceneManager::Get().PopScene();
}

