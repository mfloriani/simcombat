#pragma once

#include <chrono>

namespace engine
{
	class ClockTime
	{
	public:
		ClockTime();
		~ClockTime();

		void Update();

		double GetTime();
		float GetTimeAsFloat();
		float GetDeltaTime() const { return _deltaTime; }

	private:
		float _deltaTime;
		double _lastTime;
		std::chrono::high_resolution_clock::time_point _startTime;
	};
}