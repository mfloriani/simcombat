#include "pch.h"
#include "GamePlayScene.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Networking\NetworkManager.h"
#include "Platform\Renderer.h"

#include "Systems\AISystem.h"
#include "Systems\CombatSystem.h"

#include "Entities\Warband.h"
#include "Entities\Pike.h"
#include "Entities\Legion.h"

using namespace engine;
using namespace DirectX;

GamePlayScene::GamePlayScene() : _unitType(UnitType::Unknown), _unitQty(0)
{

}

GamePlayScene::~GamePlayScene()
{
	//GAME_TRACE("Gameplay scene destructor");
}

void GamePlayScene::OnStart()
{
	//GAME_TRACE("Gameplay scene init");

	auto& engineInstance = Engine::Get();
	engineInstance.AddCustomSystem<AISystem>();
	engineInstance.AddCustomSystem<CombatSystem>();
	
	LoadEntities();

	//NetworkManager::GameHasStarted();
}

void GamePlayScene::CreateUnits(UnitType type, int quantity, XMFLOAT3 position, Direction moveDirection)
{
	switch (type)
	{
	case UnitType::Warband:		
		Warband::Create(this, position, moveDirection);
		break;

	case UnitType::Pike:
		Pike::Create(this, position, moveDirection);
		break;

	case UnitType::Legion:
		Legion::Create(this, position, moveDirection);
		break;

	default:
		ENGINE_ERROR("Unit type not selected for this peer");
		break;
	}
}

void GamePlayScene::LoadEntities()
{

#if 0

// 0 default (same type vs)
// 1 warband x pike
// 2 warband x legion
// 3 pike x legion
#define SIM_TYPE 1

#if SIM_TYPE == 0
	Warband::Create(this, XMFLOAT3(-2.f, 0.f, 0.f), Direction::Right);
	Warband::Create(this, XMFLOAT3(2.f, 0.f, 0.f), Direction::Left);

	Pike::Create(this, XMFLOAT3(-2.f, 1.f, 0.f), Direction::Right);
	Pike::Create(this, XMFLOAT3(2.f, 1.f, 0.f), Direction::Left);

	Legion::Create(this, XMFLOAT3(-2.f, -1.f, 0.f), Direction::Right);
	Legion::Create(this, XMFLOAT3(2.f, -1.f, 0.f), Direction::Left);

#elif SIM_TYPE == 1
	Warband::Create(this, XMFLOAT3(-2.f, 0.f, 0.f), Direction::Right);
	Pike::Create(this, XMFLOAT3(2.f, 0.f, 0.f), Direction::Left);
#elif SIM_TYPE == 2
	Warband::Create(this, XMFLOAT3(-2.f, 0.f, 0.f), Direction::Right);
	Legion::Create(this, XMFLOAT3(2.f, 0.f, 0.f), Direction::Left);
#elif SIM_TYPE == 3
	Pike::Create(this, XMFLOAT3(-2.f, 0.f, 0.f), Direction::Right);
	Legion::Create(this, XMFLOAT3(2.f, 0.f, 0.f), Direction::Left);
#endif

#else

	// TODO: this is a huge hack to pass data between scenes hehe
	struct SceneData
	{
		int unitType;
		int unitQty;
	};

	auto sharedData = SceneManager::Get().GetSharedData();
	auto selectedUnitData = reinterpret_cast<SceneData*>(sharedData.get());

	_unitType = static_cast<UnitType>(selectedUnitData->unitType);
	_unitQty = selectedUnitData->unitQty;
	// end hack


	GameData enemyData = NetworkManager::Get().GetStartGameData();
	int playerId = NetworkManager::Get().GetPlayerId();

	int maxUnitsLine = 10;

	int lines = 1;
	if(_unitQty > 1)
		lines = _unitQty / maxUnitsLine;

	float distanceBetweenLines = 0.5f;
	float distanceBetweenColumns = 0.2f;

	if (playerId == static_cast<int>(PlayerSide::Left))
	{
		for (int j = 0; j < lines; ++j)
		{
			float xOffset = distanceBetweenLines * j;
			for (int i = 0; i < maxUnitsLine; ++i)
			{
				float yOffset = distanceBetweenColumns * -i;
				CreateUnits(_unitType, _unitQty, XMFLOAT3(-2.f - xOffset, 1.f + yOffset, 0.f), Direction::Right);
				CreateUnits(enemyData.enemyUnitType, enemyData.enemyUnitQty, XMFLOAT3(2.f + xOffset, 1.f + yOffset, 0.f), Direction::Left);
			}
		}
	}
	else if (playerId == static_cast<int>(PlayerSide::Right))
	{
		for (int j = 0; j < lines; ++j)
		{
			float xOffset = distanceBetweenLines * j;
			for (int i = 0; i < maxUnitsLine; ++i)
			{
				float yOffset = distanceBetweenColumns * -i;
				CreateUnits(enemyData.enemyUnitType, enemyData.enemyUnitQty, XMFLOAT3(-2.f - xOffset, 1.f + yOffset, 0.f), Direction::Right);
				CreateUnits(_unitType, _unitQty, XMFLOAT3(2.f + xOffset, 1.f + yOffset, 0.f), Direction::Left);
			}
		}
	}
	else
	{
		ENGINE_ERROR("Error finding the player side based on the player id from network manager");
	}

#endif
}

void GamePlayScene::OnKeyboard(int key, bool down)
{
	if (down) return; // Ignore key down events

	switch (key)
	{
	case 80: // P = pause
		// Put code here to add a Pause screen
		break;
	case 82: // R
		Reset();
		break;
	case 85: // U
		
		break;
	case 27: // Escape
		QuitScene();
		break;
	}
}

void GamePlayScene::OnUpdate(float dt)
{
	
}

void GamePlayScene::OnImGuiRender()
{

}

void GamePlayScene::OnEvent(engine::Event* e)
{

}

void GamePlayScene::Reset()
{
	
}

//void GamePlayScene::SetUnitSelection(UnitType unitType, int unitQty)
//{
//	_unitType = unitType;
//	_unitQty = unitQty;
//}
