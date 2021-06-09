#include "pch.h"
#include "Semaphore.h"

using namespace engine;

Semaphore::Semaphore() : _flag(0)
{
}

Semaphore::~Semaphore()
{
}

void Semaphore::Wait()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_conditional.wait(lock, [&]() { return _flag > 0; });
	--_flag;
}

void Semaphore::Signal()
{
	std::unique_lock<std::mutex> lock(_mutex);
	++_flag;
	_conditional.notify_one();
}
