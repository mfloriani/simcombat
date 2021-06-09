#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace engine;

void SceneManager::OnKeyboard(int key, bool down)
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->OnKeyboard(key, down);
	}
}

void SceneManager::OnUpdate(float deltaTime)
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->OnUpdate(deltaTime);
	}
}

void SceneManager::OnImGuiRender()
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->OnImGuiRender();
	}
}

void engine::SceneManager::PopScene()
{
	_scenesStack.pop();
}

void SceneManager::PushScene(std::string id)
{
	ENGINE_TRACE("SceneManager::PushScene thread_id {0}", std::this_thread::get_id());
	
	auto it = _scenesMap.find(id);
	if (it != _scenesMap.end())
	{
		_scenesStack.push(it->second);
		it->second->Init();
		it->second->OnStart();
	}
	else
	{
		ENGINE_WARN("The scene {0} is not registered", id);
	}
}

void SceneManager::RegisterScene(std::string id, std::shared_ptr<Scene> s)
{
	if (_scenesMap.find(id) == _scenesMap.end())
	{
		_scenesMap.insert(std::make_pair(id, s));
	}
	else
	{
		ENGINE_WARN("The scene {0} already exists", id);
	}
}

void SceneManager::DeregisterScene(std::string id)
{
	if (_scenesMap.find(id) != _scenesMap.end())
	{
		_scenesMap.erase(id);
	}
}
