#include "Engine.h"
#include "Scenes\MainMenuScene.h"
#include "Scenes\GamePlayScene.h"

using namespace engine;

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Enable run-time memory check for debug builds.
#endif

	return WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	engine::Log::Init();

	auto &engineInstance = Engine::Get();
	engineInstance.RegisterScene("MainMenu", std::make_shared<MainMenuScene>());
	engineInstance.RegisterScene("GamePlay", std::make_shared<GamePlayScene>());
	
	engineInstance.MainThread(hInstance, "MainMenu");

	return 0;
}