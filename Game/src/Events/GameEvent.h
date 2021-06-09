#pragma once

#include "Events\Event.h"
#include "ECS\Entity.h"

struct PushBackEvent : public engine::Event
{
	PushBackEvent(engine::Entity* e)
		: engine::Event(engine::EventType::PushBack), entity(e)
	{}

	engine::Entity* entity;
};