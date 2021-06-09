#pragma once

#include <vector>
#include <bitset>

#include "Constants.h"
#include "Component.h"
#include "Events\Event.h"
#include "Events\Observer.h"

namespace engine
{
	class Scene;
	struct Event;

	class System : public IObserver
	{
	public:
		System();
		virtual ~System() = default;

		virtual bool Init() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnEvent(Event* e) = 0;

		void AddEntity(Entity e);
		void RemoveEntity(Entity e);
		
		template<typename T>
		void RequireComponent()
		{
			const auto id = ComponentType<T>::GetID();
			_signature.set(id);
		}

		ECSSignature GetSignature() const { return _signature; }

	protected:
		std::vector<Entity> _entities;
		ECSSignature        _signature;


	};
}