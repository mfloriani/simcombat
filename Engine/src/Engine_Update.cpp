#include "pch.h"
#include "Engine.h"
#include "Scene.h"

#include "ECS\Systems\CollisionSystem.h"
#include "ECS\Systems\PhysicsSystem.h"

#include "Threading\ThreadingManager.h"

using namespace engine;

void Engine::UpdateThread()
{
	ENGINE_TRACE("Engine::UpdateThread init thread_id {0}", std::this_thread::get_id());
	ClockTime clock;
	
	_updateThread_SystemMgr->Add<PhysicsSystem>();
	_updateThread_SystemMgr->Add<CollisionSystem>();

	using hiResClock = std::chrono::high_resolution_clock;
	using MS = std::chrono::duration<float, std::milli>;

	//game->OnUpdate(dt);
	while (_isRunning)
	{
		float frametime = 1000.f / Engine::UpdateFrameRate;
		auto start = hiResClock::now();

		clock.Update();
		float dt = clock.GetDeltaTime();

		UpdateThread_HandleEvents();
		_updateThread_SystemMgr->OnUpdate(dt);
		SceneManager::Get().GetCurrentScene()->GetEntityManager()->OnUpdate();
		
		UpdateThread_SignalToRender();
		UpdateThread_HandleEvents();

		auto end = hiResClock::now();
		MS elapsed = end - start;
		float sleepTime = frametime - elapsed.count();
		auto ms = MS(sleepTime);
		std::this_thread::sleep_for(ms);
		//ENGINE_TRACE("sleepTime {0} | {1} | {2} | {3}", sleepTime, frametime, elapsed.count(), Engine::UpdateFrameRate);

		UpdateThread_WaitForRender();
	}


	ENGINE_TRACE("Engine::UpdateThread exit");
}



void Engine::UpdateThread_HandleEvents()
{
	_updateThread_EventQ.SwapBuffers(); // swap event back buffer
	auto& q = _updateThread_EventQ.GetReadContainer();

	if (q.empty())
		return;

	std::shared_ptr<Event> event = nullptr;
	while (!q.empty())
	{
		event = q.front();
		q.pop();

		switch (event->type)
		{
		case EventType::KeyDown:
		{
			auto e = std::static_pointer_cast<KeyDownEvent>(event);
			ENGINE_TRACE("UpdateThread_HandleEvents KeyDown {0}", e->key);
			break;
		}
		case EventType::KeyUp:
		{
			auto e = std::static_pointer_cast<KeyUpEvent>(event);
			ENGINE_TRACE("UpdateThread_HandleEvents KeyUp {0}", e->key);
			break;
		}
		case EventType::EntityCreated:
		{
			auto e = std::static_pointer_cast<EntityCreatedEvent>(event);
			ENGINE_TRACE("UpdateThread_HandleEvents EntityCreated {0}", e->entity.GetId());

			_updateThread_SystemMgr->EntityCreated(e->entity, e->entitySignature);

			break;
		}
		case EventType::EntityDestroyed:
		{
			auto e = std::static_pointer_cast<EntityDestroyedEvent>(event);
			ENGINE_TRACE("UpdateThread_HandleEvents EntityDestroyed {0}", e->entity.GetId());

			_updateThread_SystemMgr->EntityDestroyed(e->entity);

			break;
		}
		case EventType::LoadScene:
		{
			auto e = std::static_pointer_cast<LoadSceneEvent>(event);
			ENGINE_TRACE("UpdateThread_HandleEvents LoadSceneEvent {0}", e->sceneId);

			SceneManager::Get().PushScene(e->sceneId);

			break;
		}
		case EventType::CPUAffinity:
		{
			auto e = std::static_pointer_cast<CPUAffinityEvent>(event);
			ENGINE_TRACE("UpdateThread_HandleEvents CPUAffinityEvent {0}", e->cpu);

			ThreadingManager::Get().SetCPUAffinity(static_cast<CPU>(e->cpu));

			break;
		}
		default:
			break;
		}

	}
}
