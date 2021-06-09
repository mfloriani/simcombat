#include "pch.h"
#include "EventManager.h"
#include "Observer.h"

#include <mutex>

using namespace engine;

EventManager::~EventManager()
{
	//ENGINE_TRACE("EventManager destructor");
}

void EventManager::Subscribe(EventType type, IObserver* observer)
{
	ENGINE_TRACE("EventManager::Subscribe {0}", std::this_thread::get_id());
	//std::lock_guard<std::recursive_mutex> lock(_mutex);
	_callbacks[type].push_back(observer); // TODO: avoid duplication
}

void EventManager::Unsubscribe(EventType type, IObserver* observer)
{
	//std::lock_guard<std::recursive_mutex> lock(_mutex);
	if (_callbacks.find(type) != _callbacks.end())
	{
		auto& observers = _callbacks[type];
		auto it = std::find(observers.begin(), observers.end(), observer);
		if (it != observers.end())
		{
			observers.erase(it);
		}
	}
}

void EventManager::Trigger(Event* event)
{
	ENGINE_TRACE("EventManager::Trigger {0}", std::this_thread::get_id());
	//std::lock_guard<std::recursive_mutex> lock(_mutex);
	for (auto observer : _callbacks[event->type])
	{
		ENGINE_TRACE("EventManager::Trigger calling OnEvent on observer");
		observer->OnEvent(event);
	}
}
