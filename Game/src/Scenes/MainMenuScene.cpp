#include "pch.h"

#include "ImGui\imgui.h"

#include "MainMenuScene.h"
#include "GamePlayScene.h"
#include "SceneManager.h"

#include "ECS\Components\TransformComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\RigidbodyComponent.h"
#include "ECS\Systems\RenderingSystem.h"

#include "Events\EventManager.h"
#include "Events\EngineEvent.h"

#include "Networking\NetworkManager.h"

#include "Engine.h"

using namespace engine;
using namespace DirectX;


MainMenuScene::MainMenuScene()
{
	
}

MainMenuScene::~MainMenuScene()
{
	//GAME_TRACE("Main Menu destructor");
}

void MainMenuScene::OnStart()
{
	//EventManager::Get().Subscribe(EventType::StartGame, this);
}

void MainMenuScene::OnKeyboard(int key, bool down)
{
	if (down) return; // Ignore key down events

	switch (key)
	{
	case 27: // Escape
		QuitScene(); // Leave menu
		break;
	default:
		break;
	}
}

void MainMenuScene::OnUpdate(float deltaTime)
{
	
}


static bool showHostWindow = false;
static bool showJoinWindow = false;
static bool masterReady = false;

static int selectedUnitType = static_cast<int>(UnitType::Warband);
static int selectedUnitQty = 1;

void MainMenuScene::OnImGuiRender()
{
	//ENGINE_TRACE("MainMenuScene::OnImGuiRender");
	//ImGui::ShowDemoWindow(&show_demo_window);

	{
		bool open = true;
		const float PAD = 10.0f;
		static int corner = 0;
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		if (corner != -1)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
			window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
			window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
			window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			window_flags |= ImGuiWindowFlags_NoMove;
		}

		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		if (ImGui::Begin("Example: Simple overlay", &open, window_flags))
		{
			//ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
			//ImGui::Separator();
			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse Position: <invalid>");

			ImGui::Text("UI %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Render %.3f ms/frame (%.1f FPS)", 1000.0f / Engine::RenderFrameRate, Engine::RenderFrameRate);
			ImGui::Text("Update %.3f ms/frame (%.1f FPS)", 1000.0f / Engine::UpdateFrameRate, Engine::UpdateFrameRate);
			ImGui::Text("Network %.3f ms/frame (%.1f FPS)", 1000.0f / Engine::NetworkFrameRate, Engine::NetworkFrameRate);
			
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
				if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
				if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
				if (open && ImGui::MenuItem("Close")) open = false;
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	{
		ImGui::Begin("Thread Frequencies (Hz)");
		ImGui::InputFloat("Render", &Engine::RenderFrameRate, 30.00f, 100.0f, "%.3f");
		ImGui::InputFloat("Update", &Engine::UpdateFrameRate, 30.00f, 100.0f, "%.3f");
		ImGui::InputFloat("Network", &Engine::NetworkFrameRate, 10.00f, 100.0f, "%.3f");
		ImGui::End();
	}

	{
		ImGui::Begin("Main Menu");

		if (ImGui::Button("Single"))
		{
			ENGINE_TRACE("MainMenuScene::OnImGuiRender thread_id {0}", std::this_thread::get_id());
			auto ev = std::make_shared<LoadSceneEvent>("GamePlay");

			Engine::Get().BroadcastEvent(ev);
		}

		if (ImGui::Button("Host"))
		{
			if (!showHostWindow)
			{
				HandleHostGame();
			}

			showHostWindow = true;
			showJoinWindow = false;
		}

		if (ImGui::Button("Join"))
		{
			if (!showJoinWindow)
			{
				HandleJoinGame();
			}

			showHostWindow = false;
			showJoinWindow = true;
		}

		ImGui::End();


		if (showHostWindow) // hosting game
		{
			ImGui::Begin("Hosting a game", &showHostWindow);
			ImGui::Text("Waiting for other player to join...");

			
			ImGui::RadioButton("Warband", &selectedUnitType, static_cast<int>(UnitType::Warband)); ImGui::SameLine();
			ImGui::RadioButton("Pike", &selectedUnitType, static_cast<int>(UnitType::Pike)); ImGui::SameLine();
			ImGui::RadioButton("Legion", &selectedUnitType, static_cast<int>(UnitType::Legion));

			
			ImGui::RadioButton("1", &selectedUnitQty, 1); ImGui::SameLine();
			ImGui::RadioButton("10", &selectedUnitQty, 10); ImGui::SameLine();
			ImGui::RadioButton("20", &selectedUnitQty, 20); ImGui::SameLine();
			ImGui::RadioButton("30", &selectedUnitQty, 30); ImGui::SameLine();
			ImGui::RadioButton("50", &selectedUnitQty, 50); ImGui::SameLine();
			ImGui::RadioButton("100", &selectedUnitQty, 100); ImGui::SameLine();
			ImGui::RadioButton("1000", &selectedUnitQty, 1000);

			//static bool ready = true;
			//ImGui::Checkbox("Ready", &ready);

			if (ImGui::Button("Ready") && !masterReady)
			{
				masterReady = true;
				HandlePeerReady();
			}

			if (masterReady)
			{
				if (ImGui::Button("Start"))
				{
					HandleStartGame();
				}
			}
			ImGui::End();
		}
		else if (showJoinWindow) // joining game
		{
			ImGui::Begin("Joining a game", &showJoinWindow);
			ImGui::Text("I'm ready! Waiting for the host to start the game...");

			ImGui::RadioButton("Warband", &selectedUnitType, static_cast<int>(UnitType::Warband)); ImGui::SameLine();
			ImGui::RadioButton("Pike", &selectedUnitType, static_cast<int>(UnitType::Pike)); ImGui::SameLine();
			ImGui::RadioButton("Legion", &selectedUnitType, static_cast<int>(UnitType::Legion));

			ImGui::RadioButton("1", &selectedUnitQty, 1); ImGui::SameLine();
			ImGui::RadioButton("10", &selectedUnitQty, 10); ImGui::SameLine();
			ImGui::RadioButton("20", &selectedUnitQty, 20); ImGui::SameLine();
			ImGui::RadioButton("30", &selectedUnitQty, 30); ImGui::SameLine();
			ImGui::RadioButton("50", &selectedUnitQty, 50); ImGui::SameLine();
			ImGui::RadioButton("100", &selectedUnitQty, 100); ImGui::SameLine();
			ImGui::RadioButton("1000", &selectedUnitQty, 1000);

			static bool peerReady = false;
			if (ImGui::Button("Ready") && !peerReady)
			{
				peerReady = true;
				HandlePeerReady();
			}

			if (peerReady)
			{
				//HandleStartGame();
			}
			ImGui::End();
		}
	}
}

void MainMenuScene::OnEvent(Event* e)
{
	switch (e->type)
	{
	case EventType::StartGame:
	{
		//auto ev = static_cast<StartGameEvent*>(e);

		//auto gameScene = std::make_shared<GamePlayScene>();
		//gameScene->SetUnitSelection(static_cast<UnitType>(selectedUnitType), selectedUnitQty);
		//LoadScene(gameScene); // Play game

		break;
	}
	default:
		break;
	}
}

void MainMenuScene::HandleStartGame()
{
	auto ev = std::make_shared<StartGameEvent>();
	Engine::Get().BroadcastEvent(ev);
}

void MainMenuScene::HandleHostGame()
{
	auto ev = std::make_shared<HostGameEvent>();
	Engine::Get().BroadcastEvent(ev);
}

void MainMenuScene::HandleJoinGame()
{
	auto ev = std::make_shared<JoinGameEvent>();
	Engine::Get().BroadcastEvent(ev);
}

void MainMenuScene::HandlePeerReady()
{
	auto ev = std::make_shared<PeerReadyEvent>(
		NetworkManager::Get().GetName(),
		static_cast<UnitType>(selectedUnitType),
		selectedUnitQty
	);
	Engine::Get().BroadcastEvent(ev);
}