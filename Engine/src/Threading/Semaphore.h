#pragma once

#include <mutex>
#include <condition_variable>

namespace engine
{
	class Semaphore
	{
	public:
		Semaphore();
		~Semaphore();

		void Wait();
		void Signal();

	private:
		std::mutex _mutex;
		std::condition_variable _conditional;
		unsigned short _flag;
	};
}