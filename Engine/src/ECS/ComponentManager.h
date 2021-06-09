#pragma once


#include <vector>
#include <unordered_map>
#include <memory>
#include <set>
#include <typeinfo>

#include "Entity.h"
#include "Component.h"

namespace engine
{
	class IComponentManager
	{
	public:
		virtual ~IComponentManager() = default;
	};

	template<class T>
	class ComponentManager : public IComponentManager
	{
	public:
		ComponentManager()
		{
			//ENGINE_TRACE("Constructor {0}", GetClassType());
			_components.reserve(ENTITY_INIT_BUFFER);
		}

		~ComponentManager() 
		{
			//ENGINE_TRACE("Destructor {0}", GetClassType());
		}

		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator=(const ComponentManager&) = delete;

		const char* GetClassType() { return typeid(this).name(); }

		template<typename... TArgs>
		void Add(EntityID id, TArgs&&... args)
		{
			// Prevent adding the same component more than once
			auto it = _entityComponentMap.find(id);
			if (it != _entityComponentMap.end())
			{
				ENGINE_WARN("Entity [{0}] already has the Component [{1}]! Skipping new component...", id, typeid(T).name());
				return;
			}

			T component(std::forward<TArgs>(args)...);
			component.parent = id;
			component.typeId = ComponentType<T>::GetID();

			auto& comp = _components.emplace_back(std::move(component));
			_entityComponentMap[id] = &comp;
		}

		void Remove(EntityID id)
		{
			auto it = _entityComponentMap.find(id);
			if (it != _entityComponentMap.end())
			{
				//std::swap(*it, )
			}
		}
		
		std::vector<T>& GetComponents() const
		{
			return _components;
		}

		T* GetComponent(EntityID id)
		{
			auto it = _entityComponentMap.find(id);
			if (it != _entityComponentMap.end())
			{
				return it->second;
			}
			return nullptr;
		}		

	private:
		std::vector<T> _components;
		std::unordered_map<EntityID, T*> _entityComponentMap;
	};

}
