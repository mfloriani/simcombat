#include "pch.h"
#include "Engine.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_impl_dx11.h"

#include "ECS\Systems\RenderingSystem.h"

#include "Threading\ThreadingManager.h"

using namespace engine;

void Engine::RenderThread()
{
	ENGINE_TRACE("Engine::RenderThread init thread_id {0}", std::this_thread::get_id());
	ClockTime clock;

#if _DX
	_renderer = std::make_unique<RendererDX>(_window->GetHwnd());

	if (!_renderer->Init())
	{
		ENGINE_ERROR("Error initializing DirectX renderer!");
		return;
	}

	ResourceManager::Get().Init(_renderer.get()); // TODO: it shouldn't be called here but it is throwing error otherwise

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(_window->GetHwnd());
	ImGui_ImplDX11_Init(_renderer->GetDevice(), _renderer->GetContext());

	_renderThread_SystemMgr->Add<RenderingSystem>(_renderer.get());

	using hiResClock = std::chrono::high_resolution_clock;
	using MS = std::chrono::duration<float, std::milli>;

	while (_isRunning)
	{
		float frametime = 1000.f / Engine::RenderFrameRate;
		auto start = hiResClock::now();

		clock.Update();

		RenderThread_HandleEvents();

		if (_stopAllThreads)
			break;

		RenderThread_WaitForUpdate();

		_renderer->ClearScreen();

		_renderThread_SystemMgr->OnUpdate(clock.GetDeltaTime());

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		SceneManager::Get().OnImGuiRender();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		_renderer->SwapBuffers();

		RenderThread_SignalToUpdate();
		RenderThread_HandleEvents();

		auto end = hiResClock::now();
		MS elapsed = end - start;
		float sleepTime = frametime - elapsed.count();
		auto ms = MS(sleepTime);
		std::this_thread::sleep_for(ms);
		//ENGINE_TRACE("sleepTime {0} | {1} | {2} | {3}", sleepTime, frametime, elapsed.count(), Engine::RenderFrameRate);
	}


	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#elif _GL
	
#endif

	ENGINE_TRACE("Engine::RenderThread exit");
}

void Engine::RenderThread_HandleEvents()
{
	_renderThread_EventQ.SwapBuffers(); // swap event back buffer
	auto& q = _renderThread_EventQ.GetReadContainer();

	if (q.empty())
		return;

	std::shared_ptr<Event> event = nullptr;
	while (!q.empty())
	{
		event = q.front();
		q.pop();

		switch (event->type)
		{
		case EventType::EntityCreated:
		{
			auto e = std::static_pointer_cast<EntityCreatedEvent>(event);
			ENGINE_TRACE("RenderThread_HandleEvents EntityCreated {0}", e->entity.GetId());

			_renderThread_SystemMgr->EntityCreated(e->entity, e->entitySignature);

			break;
		}
		case EventType::EntityDestroyed:
		{
			auto e = std::static_pointer_cast<EntityDestroyedEvent>(event);
			ENGINE_TRACE("RenderThread_HandleEvents EntityDestroyed {0}", e->entity.GetId());

			_renderThread_SystemMgr->EntityDestroyed(e->entity);

			break;
		}
		case EventType::CPUAffinity:
		{
			auto e = std::static_pointer_cast<CPUAffinityEvent>(event);
			ENGINE_TRACE("RenderThread_HandleEvents CPUAffinityEvent {0}", e->cpu);

			ThreadingManager::Get().SetCPUAffinity(static_cast<CPU>(e->cpu));

			break;
		}
		default:
			break;
		}

	}
}