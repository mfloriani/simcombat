#pragma once

#include "Scene.h"

class MainMenuScene : public engine::Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene();

public:
	virtual void OnStart() override;
	virtual void OnKeyboard(int key, bool down) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(engine::Event* e) override;

	void HandleStartGame();
	void HandleHostGame();
	void HandleJoinGame();
	void HandlePeerReady();
};

