#pragma once
#include "Scene.h"
#include "Constants.h"
#include <DirectXMath.h>

class GamePlayScene : public engine::Scene
{
public:
	GamePlayScene();
	virtual ~GamePlayScene();

public:
	virtual void OnStart() override;
	virtual void OnKeyboard(int key, bool down) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(engine::Event* e) override;

	void CreateUnits(engine::UnitType type, int quantity, DirectX::XMFLOAT3 position, engine::Direction moveDirection);
	void LoadEntities();
	void Reset();
	//void SetUnitSelection(engine::UnitType unitType, int unitQty);

private:
	engine::UnitType _unitType;
	int _unitQty;
};

