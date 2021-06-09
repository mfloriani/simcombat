#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

#include "System.h"

namespace engine
{
	class SystemManager
	{
	
		using SystemList = std::vector<std::shared_ptr<System>>;
		using SystemTypeMap = std::unordered_map<std::type_index, std::shared_ptr<System>>;

	public:
		SystemManager();
		~SystemManager();

		template<typename T, typename ...TArgs>
		void Add(TArgs&& ...args)
		{
			auto type = std::type_index(typeid(T));
			if (_systemsTypeMap.find(type) == _systemsTypeMap.end())
			{
				std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<TArgs>(args)...);			
				_systemsTypeMap.insert(std::make_pair(type, newSystem));
				_systems.push_back(newSystem);
			}
			else
			{
				ENGINE_WARN("System already exists {0}", type.name());
			}
		}

		template<typename T>
		void Remove()
		{
			auto system = _systemsTypeMap.find(std::type_index(typeid(T)));
			if (system != _systemsTypeMap.end())
			{
				_systemsTypeMap.erase(system);
				auto it = std::find(_systems.begin(), _systems.end(), system->second);
				_systems.erase(it); // TODO: check if is really removing the shared_ptr
			}
		}

		template <typename T>
		bool Has() const 
		{
			return _systems.find(std::type_index(typeid(T))) != _systems.end();
		}

		template <typename T>
		T& Get() const 
		{
			auto system = _systemsTypeMap.find(std::type_index(typeid(T)));
			return *(std::static_pointer_cast<T>(system->second));
		}

		const SystemList& GetAll() const { return _systems; }

		void EntityCreated(Entity e, ECSSignature signature);
		void EntityDestroyed(Entity e);

		void OnUpdate(float dt);

	private:
		SystemList    _systems;
		SystemTypeMap _systemsTypeMap;
	};
}