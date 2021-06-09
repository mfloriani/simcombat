#pragma once

#include <vector>
#include <set>
#include <bitset>
#include <typeindex>

#include "Constants.h"
#include "Entity.h"
#include "ComponentManager.h"
#include "Events\EngineEvent.h"

namespace engine
{
	class Scene;

	class EntityManager
	{
		using EntitySignatureMap = std::unordered_map<EntityID, ECSSignature>;

	public:
		EntityManager() : _numEntities(0)
		{
			//ENGINE_TRACE("EntityManager construct");

			_entities.reserve(ENTITY_INIT_BUFFER);
		}

		~EntityManager()
		{
			//ENGINE_TRACE("EntityManager destructor");

			
		}

	public:
		Entity CreateEntity()
		{
			auto id = GetNextId();
			Entity e(this, id);

			_entities.push_back(id);
			_entitiesToAdd.insert(id);

			return e;
		}

		// this should not be used to create entities, only to assemble an Entity object 
		Entity GetEntity(EntityID id)
		{
			// TODO: check that id exists
			return Entity(this, id);
		}

		void DestroyEntity(EntityID id)
		{
			_entitiesToRemove.insert(id);
		}



		template<typename T, typename... TArgs>
		void AddComponent(EntityID id, TArgs&& ...args)
		{
			auto typeId = std::type_index(typeid(T));
			auto componentId = ComponentType<T>::GetID();

			if (_componentManagers.find(typeId) == _componentManagers.end())
			{
				_componentManagers[typeId] = std::make_shared<ComponentManager<T>>();
			}
			
			std::shared_ptr<ComponentManager<T>> componentManager = std::static_pointer_cast<ComponentManager<T>>(_componentManagers[typeId]);
			componentManager->Add(id, std::forward<TArgs>(args)...);

			RegisterComponent(id, componentId);
		}

		template<typename T>
		void RemoveComponent(EntityID id)
		{
			auto typeId = std::type_index(typeid(T));
			std::shared_ptr<ComponentManager<T>> componentManager = std::static_pointer_cast<ComponentManager<T>>(_componentManagers[typeId]);
			componentManager->Remove(id);

			auto componentId = ComponentType<T>::GetID();
			DeregisterComponent(id, componentId);
		}

		template<typename T>
		T* GetComponent(EntityID id)
		{
			auto typeId = std::type_index(typeid(T));
			std::shared_ptr<ComponentManager<T>> componentManager = std::static_pointer_cast<ComponentManager<T>>(_componentManagers[typeId]);

			return componentManager->GetComponent(id);
		}

		void AddToSystems(EntityID id);
		void RemoveFromSystems(EntityID id);

		void RegisterComponent(EntityID entityId, ComponentTypeID componentId)
		{
			_entitySignatureMap[entityId].set(componentId);
		}

		void DeregisterComponent(EntityID entityId, ComponentTypeID componentId)
		{
			_entitySignatureMap[entityId].set(componentId, false);
		}

		void OnUpdate()
		{
			for (auto id : _entitiesToRemove)
			{
				RemoveFromSystems(id);
			}
			_entitiesToRemove.clear();

			for (auto id : _entitiesToAdd)
			{
				AddToSystems(id);
			}
			_entitiesToAdd.clear();
		}

		//std::set<Entity>& GetEntitiesToRemove() { return _entitiesToRemove; }
		//std::set<Entity>& GetEntitiesToAdd() { return _entitiesToAdd; }
		//EntitySignatureMap& GetEntitiesSignatureMap() { return _entitySignatureMap; }

	private:
		size_t GetNextId() { return _numEntities++; }

	private:
		size_t _numEntities;
		
		std::vector<EntityID> _entities;
		std::set<EntityID>    _entitiesToRemove;
		std::set<EntityID>    _entitiesToAdd;

		EntitySignatureMap _entitySignatureMap;
		std::unordered_map<std::type_index, std::shared_ptr<IComponentManager>> _componentManagers;

	};


}
