#include "pch.h"
#include "CombatSystem.h"
#include "ECS\EntityManager.h"
#include "Components\CombatComponent.h"
#include "Events\EngineEvent.h"
#include "Events\GameEvent.h"
#include "Events\EventManager.h"

#include <cstdlib>
#include <time.h>

using namespace engine;

CombatSystem::CombatSystem() : System()
{
	RequireComponent<CombatComponent>();

	EventManager::Get().Subscribe(EventType::Combat, this);

	srand(static_cast<unsigned int>(time(nullptr)));
}

CombatSystem::~CombatSystem()
{
}

bool CombatSystem::Init()
{
	return true;
}

void CombatSystem::OnUpdate(float dt)
{
	//ENGINE_TRACE("CombatSystem::OnUpdate thread_id {0}", std::this_thread::get_id());

}

void CombatSystem::OnEvent(engine::Event* e)
{
	//ENGINE_TRACE("CombatSystem::OnEvent {0}", e->type);

	switch (e->type)
	{
	case EventType::Combat:
	{
		HandleCombatEvent(e);
		break;
	}
	default:
		break;
	}
}

void CombatSystem::HandleCombatEvent(engine::Event* e)
{
	auto ev = static_cast<CombatEvent*>(e);

	auto weaponA = ev->entityA->GetComponent<CombatComponent>();
	auto weaponB = ev->entityB->GetComponent<CombatComponent>();

	int attackA = (rand() % 6 + 1) + weaponA->attack;
	int attackB = (rand() % 6 + 1) + weaponB->attack;

	GAME_TRACE("CombatEvent {0} x {1}", attackA, attackB);

	if (attackA > attackB)
	{
		bool isWinnerWarband = weaponA->unitType == UnitType::Warband;
		bool isLoserWarband = weaponB->unitType == UnitType::Warband;
		bool doubleScore = attackA >= (attackB * 2);
		bool isCritical = doubleScore || (isWinnerWarband && !isLoserWarband);

		if (isCritical)
		{
			GAME_WARN("A did CRITICAL");
			DeathEvent e(ev->entityB);
			EventManager::Get().Trigger(&e);
		}
		else
		{
			GAME_WARN("A PUSHED BACK");
			PushBackEvent e(ev->entityB);
			EventManager::Get().Trigger(&e);
		}
	}
	else if(attackA < attackB)
	{
		bool isWinnerWarband = weaponB->unitType == UnitType::Warband;
		bool isLoserWarband = weaponA->unitType == UnitType::Warband;
		bool doubleScore = attackB >= (attackA * 2);
		bool isCritical = doubleScore || (isWinnerWarband && !isLoserWarband);
		
		if (isCritical)
		{
			GAME_WARN("B did CRITICAL");
			DeathEvent e(ev->entityA);
			EventManager::Get().Trigger(&e);
		}
		else
		{
			GAME_WARN("B PUSHED BACK");
			PushBackEvent e(ev->entityA);
			EventManager::Get().Trigger(&e);
		}
	}
	else
	{
		GAME_WARN("TIE");
	}
}
