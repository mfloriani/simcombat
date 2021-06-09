#include "pch.h"
#include "Entity.h"
#include "EntityManager.h"

using namespace engine;

void Entity::DestroyItSelf()
{
	_entityMgr->DestroyEntity(_id);
}
