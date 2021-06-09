#include "pch.h"
#include "Engine.h"

#include "ECS\Systems\CollisionSystem.h"
#include "ECS\Systems\PhysicsSystem.h"

#include "Threading\ThreadingManager.h"

using namespace engine;

float Engine::RenderFrameRate = 60.f;
float Engine::UpdateFrameRate = 60.f;
float Engine::NetworkFrameRate = 10.f;

Engine::~Engine()
{
	Quit();
}

bool Engine::Init(HINSTANCE hInstance, const std::string& sceneId)
{
#if _DX
	GAME_INFO("DirectX API");

	if (!Config::Get().LoadConfigFile("config.txt"))
		return false;

	_window = std::make_unique<WindowDX>(this);
	if (!_window->Init(hInstance, WndProc))
	{
		GAME_ERROR("Failed to init DirectX window");
		return 1;
	}

	_updateThread_SystemMgr = std::make_unique<SystemManager>();
	_renderThread_SystemMgr = std::make_unique<SystemManager>();
	_networkThread_SystemMgr = std::make_unique<SystemManager>();

	_stopAllThreads.store(false);

	_networkThread = std::thread(&Engine::NetworkThread, this);
	_updateThread = std::thread(&Engine::UpdateThread, this);
	_renderThread = std::thread(&Engine::RenderThread, this);

	SceneManager::Get().PushScene(sceneId);


#elif _GL

	GAME_INFO("OpenGL API");

	std::shared_ptr<WindowGL> windowGL = std::make_shared<WindowGL>(this);
	windowGL->Init(hInstance, nullptr);
	auto glfwWindow = windowGL->GetWindow();

	while (!glfwWindowShouldClose(glfwWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}
	glfwTerminate();

#endif
	return true;
}

void Engine::Quit() noexcept
{
	_networkThread.join();
	_updateThread.join();
	_renderThread.join();

	ENGINE_TRACE("Engine::MainThread exit");
}

void Engine::MainThread(HINSTANCE hInstance, const std::string& startScene)
{
	ENGINE_TRACE("Engine::MainThread init thread_id {0}", std::this_thread::get_id());

	if (!Init(hInstance, startScene))
	{
		return;
	}

	MSG  msg;
	msg.message = WM_NULL;
	while (_isRunning)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				ENGINE_TRACE("WM_QUIT");
				_isRunning = false;
				break;
			}
		}

		//ClockTime::Get().Update();
		//float dt = ClockTime::Get().GetDeltaTime();

		MainThread_HandleEvents();
	}

}

void Engine::MainThread_HandleEvents()
{
	_mainThread_EventQ.SwapBuffers(); // swap event back buffer
	auto& q = _mainThread_EventQ.GetReadContainer();

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
			ENGINE_TRACE("MainThread_HandleEvents KeyDown {0}", e->key);

			if (e->key == VK_ESCAPE)
			{
				_isRunning = false;
				_stopAllThreads.store(true);
				_renderSemaphore.Signal();
				_updateSemaphore.Signal();
			}

			break;
		}
		case EventType::KeyUp:
		{
			auto e = std::static_pointer_cast<KeyUpEvent>(event);
			ENGINE_TRACE("MainThread_HandleEvents KeyUp {0}", e->key);
			break;
		}
		case EventType::HostGame:
		{
			auto e = std::static_pointer_cast<HostGameEvent>(event);
			ENGINE_TRACE("MainThread_HandleEvents HostGame");
			
			auto renderCPU = std::make_shared<CPUAffinityEvent>(CPU_1);
			_renderThread_EventQ.AddItem(renderCPU);

			auto updateCPU = std::make_shared<CPUAffinityEvent>(CPU_2);
			_updateThread_EventQ.AddItem(updateCPU);

			auto networkCPU = std::make_shared<CPUAffinityEvent>(CPU_3);
			_networkThread_EventQ.AddItem(networkCPU);

			break;
		}
		case EventType::JoinGame:
		{
			auto e = std::static_pointer_cast<JoinGameEvent>(event);
			ENGINE_TRACE("MainThread_HandleEvents JoinGame");

			auto renderCPU = std::make_shared<CPUAffinityEvent>(CPU_5);
			_renderThread_EventQ.AddItem(renderCPU);

			auto updateCPU = std::make_shared<CPUAffinityEvent>(CPU_6);
			_updateThread_EventQ.AddItem(updateCPU);

			auto networkCPU = std::make_shared<CPUAffinityEvent>(CPU_7);
			_networkThread_EventQ.AddItem(networkCPU);

			break;
		}
		default:
			break;
		}

	}	
}


void Engine::UpdateThread_WaitForRender()
{
	_updateSemaphore.Wait();
}

void Engine::RenderThread_WaitForUpdate()
{
	_renderSemaphore.Wait();
}

void Engine::UpdateThread_SignalToRender()
{
	_renderSemaphore.Signal();
}

void Engine::RenderThread_SignalToUpdate()
{
	_updateSemaphore.Signal();
}


void Engine::RegisterScene(std::string id, std::shared_ptr<Scene> s)
{
	SceneManager::Get().RegisterScene(id, s);
}


