#include "pch.h"
#include "ThreadingManager.h"

using namespace engine;

ThreadingManager::ThreadingManager()
{
}

ThreadingManager::~ThreadingManager()
{
}

void ThreadingManager::AddThread(std::function<void()> f)
{
	_threads.emplace_back(
		std::thread(f)
	);
}

void ThreadingManager::Join()
{
	for (auto& t : _threads)
	{
		t.join();
	}
}

void ThreadingManager::SetCPUAffinity(CPU cpu)
{
	//HANDLE process = GetCurrentProcess();
	//SetProcessAffinityMask(process, processAffinityMask);
	DWORD_PTR processAffinityMask = 1 << cpu;
	SetThreadAffinityMask(GetCurrentThread(), processAffinityMask);

}
