#pragma once

#include "ECS\EntityManager.h"
#include "Events\Event.h"
#include "Events\Observer.h"

namespace engine
{
	class Renderer;
	struct Event;
	
	class Scene : public IObserver
	{
	public:
		Scene();
		virtual ~Scene();

	public:
		Entity CreateEntity();
		void DestroyEntity(Entity* e);
		
	public:
		void Init();

		virtual void OnStart() = 0;
		virtual void OnKeyboard(int key, bool down) = 0;
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event* e) = 0;
		
		void LoadScene(const std::string& sceneId);
		void QuitScene();

		EntityManager* GetEntityManager() { return _entityManager.get(); }

	private:
		std::unique_ptr<EntityManager> _entityManager;
	};
}