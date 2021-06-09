#pragma once

#include <memory>
#include <thread>
#include <queue>
#include <chrono>

#include "Log.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Config.h"

#include "Threading\BufferedContainer.h"
#include "Threading\Semaphore.h"

#include "Events\EngineEvent.h"
#include "Events\WindowEvent.h"

#include "ECS\SystemManager.h"

#if _DX
#include "Platform\DirectX\WindowDX.h"
#include "Platform\DirectX\RendererDX.h"
#elif _GL
#include "Platform\OpenGl\WindowGL.h"
#include "Platform\OpenGl\RendererGL.h"
#endif

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace engine
{
	

	class Engine : public IWindowOwner
	{
	public:
		static float RenderFrameRate;
		static float UpdateFrameRate;
		static float NetworkFrameRate;

	public:
		~Engine();

		static Engine& Get()
		{
			static Engine instance;
			return instance;
		}

		void MainThread(HINSTANCE hInstance, const std::string& startScene);
		void BroadcastEvent(std::shared_ptr<Event> e);
		
		template<typename T, typename ...TArgs>
		void AddCustomSystem(TArgs&& ...args)
		{
			_updateThread_SystemMgr->Add<T>(std::forward<TArgs>(args)...);
		}

		virtual void OnKeyDown(WPARAM key) override;
		virtual void OnKeyUp(WPARAM key) override;
		
		void RegisterScene(std::string id, std::shared_ptr<Scene> s);

		

	private:
		Engine::Engine() :
			_isRunning(true)
		{
		}

		bool Init(HINSTANCE hInstance, const std::string& sceneId);
		void Quit() noexcept;

		void NetworkThread();
		void UpdateThread();
		void RenderThread();

		void UpdateThread_HandleEvents();
		void RenderThread_HandleEvents();
		void MainThread_HandleEvents();
		void NetworkThread_HandleEvents();

		void UpdateThread_WaitForRender();
		void RenderThread_WaitForUpdate();
		void UpdateThread_SignalToRender();
		void RenderThread_SignalToUpdate();

	private:
		
#if _DX
		std::unique_ptr<RendererDX>  _renderer;
		std::unique_ptr<WindowDX>    _window;
#elif _GL
		std::unique_ptr<RendererGL>  _renderer;
		std::unique_ptr<WindowGL>    _window;
#endif
		
		bool _isRunning;

		std::unique_ptr<ResourceManager> _resourceManager;
		
		std::thread _networkThread;
		std::thread _updateThread;
		std::thread _renderThread;

		using EventPtr_t = std::shared_ptr<Event>;
		using EventQueue_t = BufferedContainer<std::queue<EventPtr_t>, EventPtr_t>;

		EventQueue_t _mainThread_EventQ;
		EventQueue_t _networkThread_EventQ;
		EventQueue_t _updateThread_EventQ;
		EventQueue_t _renderThread_EventQ;

		std::atomic<bool> _stopAllThreads;
		Semaphore _updateSemaphore;
		Semaphore _renderSemaphore;

		std::unique_ptr<SystemManager> _updateThread_SystemMgr;
		std::unique_ptr<SystemManager> _renderThread_SystemMgr;
		std::unique_ptr<SystemManager> _networkThread_SystemMgr;

		

	};
}