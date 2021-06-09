#pragma once

#include <vector>

#include "ThreadSafeQueue.h"

namespace engine
{
	template<class T>
	class ThreadPool
	{
	public:
		ThreadPool(int numThreads);
		~ThreadPool();

		void WorkerThread(ThreadSafeQueue<T>& workQueue);

	private:
		ThreadSafeQueue<T> _workQueue;
		std::vector<std::thread> _threads;
	};
	
	template<class T>
	inline ThreadPool<T>::ThreadPool(int numThreads)
	{
		for (int i = 0; i < numThreads; ++i)
		{
			_threads.push_back(std::thread(

			));
		}
	}

	template<class T>
	inline ThreadPool<T>::~ThreadPool()
	{
	}

	template<class T>
	inline void ThreadPool<T>::WorkerThread(ThreadSafeQueue<T>& workQueue)
	{
		while (true)
		{
			T task = workQueue.Front();
			workQueue.Pop();

			task();
		}
	}
}