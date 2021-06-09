#pragma once

#include <stack>
#include <map>
#include <memory>
#include <string>

namespace engine
{
	class Scene;

	class SceneManager
	{
	public:
		~SceneManager() = default;

		static SceneManager& Get()
		{
			static SceneManager instance;
			return instance;
		}

		Scene* GetCurrentScene() const { return (_scenesStack.size() > 0) ? _scenesStack.top().get() : NULL; }
		bool HasScene() { return _scenesStack.size() > 0; }

		void OnKeyboard(int key, bool down);
		void OnUpdate(float deltaTime);
		void OnImGuiRender();

		void PopScene();
		void PushScene(std::string id);

		void RegisterScene(std::string id, std::shared_ptr<Scene> s);
		void DeregisterScene(std::string id);

		void SetSharedData(std::shared_ptr<void> data) { _sharedData = data; }
		std::shared_ptr<void>& GetSharedData() { return _sharedData; }

	private:
		SceneManager() = default;

	private:
		std::stack<std::shared_ptr<Scene>>            _scenesStack;
		std::map<std::string, std::shared_ptr<Scene>> _scenesMap;
		std::shared_ptr<void> _sharedData; // hack to share data between scenes 
	};
}