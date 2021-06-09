#pragma once

namespace engine
{
	enum class EventType
	{
		Collision,
		Combat,
		Death,
		PushBack,
		HostGame,
		JoinGame,
		QuitGame,
		PeerReady,
		StartGame,
		EntityCreated,
		EntityDestroyed,
		LoadScene,

		KeyDown,
		KeyUp,
		MouseMove,

		CPUAffinity

	};

	struct Event
	{
		Event(EventType type) : type(type) {}
		EventType type;
	};
}