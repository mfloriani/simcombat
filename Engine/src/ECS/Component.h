#pragma once

#include "Entity.h"

namespace engine
{
	struct IComponentType
	{
	protected:
		static ComponentTypeID nextId;
	};

	template<class T>
	class ComponentType : public IComponentType
	{
	public:
		static ComponentTypeID GetID()
		{
			static auto id = ++nextId;
			return id;
		}
	};

	struct Component
	{
		EntityID parent;
		ComponentTypeID typeId;
	};
}