#include "pch.h"
#include "ClockTime.h"

using namespace engine;
using namespace std::chrono;

ClockTime::ClockTime() : _deltaTime(0), _lastTime(0)
{
	_startTime = high_resolution_clock::now();
	_lastTime = GetTime();
}

ClockTime::~ClockTime()
{

}

void ClockTime::Update()
{
	double currentTime = GetTime();
	_deltaTime = static_cast<float>(currentTime - _lastTime);
	_lastTime = currentTime;
 
  if (_deltaTime > 0.1f) _deltaTime = 0.1f;
}

double ClockTime::GetTime()
{
	auto now = high_resolution_clock::now();
	auto ms = duration_cast<milliseconds>(now - _startTime).count();
	return static_cast<double>(ms) / 1000;
}

float ClockTime::GetTimeAsFloat()
{
	return static_cast<float>(GetTime());
}
