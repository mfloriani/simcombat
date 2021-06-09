#pragma once

#include "Constants.h"

#include <bitset>

namespace engine
{
	class EntityManager;

	using ECSSignature = std::bitset<MAX_COMPONENTS>;

	class Entity
	{
	public:
		Entity(EntityManager* entityMgr, EntityID id) : _id(id), _entityMgr(entityMgr) {}

	public:
		template<typename T, typename... TArgs>
		void AddComponent(TArgs&& ...args)
		{
			_entityMgr->AddComponent<T>(_id, std::forward<TArgs>(args)...);
		}

		template<typename T>
		T* GetComponent()
		{
			return _entityMgr->GetComponent<T>(_id);
		}

		bool operator ==(const Entity& other) const { return _id == other._id; }
		bool operator !=(const Entity& other) const { return _id != other._id; }
		bool operator >(const Entity& other) const { return _id > other._id; }
		bool operator <(const Entity& other) const { return _id < other._id; }
		
		EntityID GetId() const { return _id; }

		void DestroyItSelf();
		
	private:
		EntityID       _id;
		EntityManager* _entityMgr;
		
	};
}
