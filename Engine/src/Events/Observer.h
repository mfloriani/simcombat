#pragma once

namespace engine
{
	struct Event;

	class IObserver
	{
	public:
		IObserver(){}
		virtual ~IObserver(){}
		virtual void OnEvent(Event* e) = 0;
	};
}