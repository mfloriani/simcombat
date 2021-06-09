#pragma once

#include <vector>
#include <thread>

namespace engine
{
	enum class PeerID
	{
		PeerA,
		PeerB
	};

	enum CPU
	{
		CPU_0,
		CPU_1,
		CPU_2,
		CPU_3,
		CPU_4,
		CPU_5,
		CPU_6,
		CPU_7,
	};

	class ThreadingManager
	{
	public:
		~ThreadingManager();

		static ThreadingManager& Get()
		{
			static ThreadingManager instance;
			return instance;
		}

		void AddThread(std::function<void()> f);

		void Join();

		void SetCPUAffinity(CPU cpu);

	private:
		ThreadingManager();

	private:
		std::vector<std::thread> _threads;
	};
}