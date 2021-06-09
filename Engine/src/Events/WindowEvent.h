#pragma once

#include "Events\Event.h"
#include <wtypes.h>

namespace engine
{
	struct KeyDownEvent : public Event
	{
		KeyDownEvent(WPARAM k) :
			Event(EventType::KeyDown),
			key(k)
		{}
		WPARAM key;
	};

	struct KeyUpEvent : public Event
	{
		KeyUpEvent(WPARAM k) :
			Event(EventType::KeyUp),
			key(k)
		{}
		WPARAM key;
	};
}