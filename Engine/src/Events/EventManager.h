#pragma once

#include <vector>
#include <unordered_map>
//#include <mutex>

#include "Event.h"

namespace engine
{
	class IObserver;

	class EventManager
	{
	public:
		~EventManager();

		static EventManager& Get()
		{
			ENGINE_TRACE("EventManager::Get {0}", std::this_thread::get_id());
			thread_local static EventManager instance; // per thread
			return instance;
		}

		void Subscribe(EventType type, IObserver* observer);
		void Unsubscribe(EventType type, IObserver* observer);
		void Trigger(Event* event);
	
	private:
		EventManager() {}

	private:
		std::unordered_map<EventType, std::vector<IObserver*>> _callbacks;
		//std::recursive_mutex _mutex; // TODO: investigate why only works with recursive_mutex
	};

	
}