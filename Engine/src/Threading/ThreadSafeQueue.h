#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace engine
{
	template<class T>
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue()
		{

		}

		~ThreadSafeQueue()
		{

		}

		void Push(T value)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push(value);
		}

		T Front()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_condition.wait(lock, [this]() {return !_queue.empty(); });

			return _queue.front();
		}

		void Pop()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_condition.wait(lock, [this]() {return !_queue.empty(); });

			_queue.pop();

			lock.unlock();
			_condition.notify_all();
		}

	private:
		std::queue<T> _queue;
		std::mutex _mutex;
		std::condition_variable _condition;

	};
}