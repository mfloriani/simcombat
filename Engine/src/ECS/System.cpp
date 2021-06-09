#include "pch.h"
#include "System.h"
#include "Scene.h"

using namespace engine;

System::System() 
{

}

void System::AddEntity(Entity e)
{
	_entities.push_back(e);
}

void System::RemoveEntity(Entity e)
{
	//ENGINE_TRACE("System::RemoveEntity [entity:{0}]", e.GetId());
	
	auto it = std::find(_entities.begin(), _entities.end(), e);
	if (it != _entities.end())
	{
		//ENGINE_TRACE("Entity removed from system");
		_entities.erase(it);
	}
}
