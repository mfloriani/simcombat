#pragma once

#include <string>

namespace engine
{
	//constexpr unsigned int SCREEN_WIDTH = 1280;
	constexpr unsigned int SCREEN_WIDTH = 1024;
	//constexpr unsigned int SCREEN_HEIGHT = 720;
	constexpr unsigned int SCREEN_HEIGHT = 576;

	typedef unsigned int ComponentTypeID;
	typedef size_t EntityID;

	constexpr unsigned int MAX_COMPONENTS = 32;
	constexpr size_t ENTITY_INIT_BUFFER = 1000;

	enum class Direction
	{
		None,
		Right,
		Left = -1
	};

	// Networking
	constexpr float TimeBetweenIntros = 1.0f;
	constexpr unsigned int BufferMaxSize = 1400;
	constexpr unsigned int MaxPlayers = 2;

	enum class UnitType
	{
		Unknown = 0,
		Warband,
		Pike,
		Legion
	};

	enum class PlayerSide
	{
		None,
		Left,
		Right
	};
}
