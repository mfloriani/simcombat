#include "pch.h"
#include "Engine.h"
#include "ECS\Systems\NetworkingSystem.h"
#include "Networking\WSA.h"
#include "Networking\NetworkManager.h"
#include "Threading\ThreadingManager.h"

using namespace engine;

void Engine::NetworkThread()
{
	ENGINE_TRACE("Engine::NetworkThread init thread_id {0}", std::this_thread::get_id());

	WSA wsa; // initialize networking

	_networkThread_SystemMgr->Add<NetworkingSystem>();
	//netSys->Init();
	
	using namespace std::chrono_literals;
	using hiResClock = std::chrono::high_resolution_clock;
	using MS = std::chrono::duration<float, std::milli>;

	while (_isRunning)
	{
		float frametime = 1000.f / Engine::NetworkFrameRate;
		auto start = hiResClock::now();
		
		NetworkThread_HandleEvents();
		_networkThread_SystemMgr->OnUpdate(frametime);

		auto end = hiResClock::now();
		MS elapsed = end - start;
		float sleepTime = frametime - elapsed.count();
		//ENGINE_TRACE("sleepTime {0} | {1} | {2} | {3}", sleepTime, frametime, elapsed.count(), Engine::NetworkFrameRate);
		auto ms = MS(sleepTime);
		std::this_thread::sleep_for(ms);
	}
	
	ENGINE_TRACE("Engine::NetworkThread exit");
}

void Engine::NetworkThread_HandleEvents()
{
	_networkThread_EventQ.SwapBuffers(); // swap event back buffer
	auto& q = _networkThread_EventQ.GetReadContainer();

	if (q.empty())
		return;

	ENGINE_TRACE("NetworkThread_HandleEvents {0}", q.size());

	std::shared_ptr<Event> event = nullptr;
	while (!q.empty())
	{
		event = q.front();
		q.pop();

		switch (event->type)
		{
		case EventType::HostGame:
		{
			auto e = std::static_pointer_cast<HostGameEvent>(event);
			ENGINE_TRACE("NetworkThread_HandleEvents HostGame");

			int port = std::stoi(Config::Get().GetConfigValue("server_port"));

			NetworkManager::Get().StaticInitAsMasterPeer(port, "Host");

			break;
		}
		case EventType::JoinGame:
		{
			auto e = std::static_pointer_cast<JoinGameEvent>(event);
			ENGINE_TRACE("NetworkThread_HandleEvents JoinGame");

			std::string server = Config::Get().GetConfigValue("server_ip");

			NetworkManager::Get().StaticInitAsPeer(server, "Guest");

			break;
		}
		case EventType::PeerReady:
		{
			auto e = std::static_pointer_cast<PeerReadyEvent>(event);
			ENGINE_TRACE("NetworkThread_HandleEvents PeerReady [{0},{1},{2}]", e->peerName, e->unitType, e->unitQty);

			NetworkManager::Get().SetPeerReady(e->unitType, e->unitQty);

			// TODO: this is not something great. I should redesign it
			struct SceneData
			{
				int unitType;
				int unitQty;
			};

			auto data = std::make_shared<SceneData>();
			data->unitType = static_cast<int>(e->unitType);
			data->unitQty = e->unitQty;

			SceneManager::Get().SetSharedData(data);
			// 

			break;
		}
		case EventType::StartGame:
		{
			auto e = std::static_pointer_cast<StartGameEvent>(event);
			//ENGINE_TRACE("NetworkThread_HandleEvents StartGameEvent");

			auto& networking = NetworkManager::Get();

			if (networking.IsReadyToStartGame())
			{
				networking.StartGame(); //master tells the peers that the game is starting
				
				auto ev = std::make_shared<LoadSceneEvent>("GamePlay");
				BroadcastEvent(ev);
			}

			break;
		}
		case EventType::CPUAffinity:
		{
			auto e = std::static_pointer_cast<CPUAffinityEvent>(event);
			ENGINE_TRACE("NetworkThread_HandleEvents CPUAffinityEvent {0}", e->cpu);

			ThreadingManager::Get().SetCPUAffinity(static_cast<CPU>(e->cpu));

			break;
		}
		default:
			break;
		}
	}
}