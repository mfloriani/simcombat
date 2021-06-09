#pragma once

#include <thread>
#include <mutex>
#include <array>

namespace engine
{
	template<class TContainer, class TItem>
	class BufferedContainer
	{
	public:
		BufferedContainer() : _bufferIndex(0)
		{

		}

		TContainer& GetReadContainer() { return _bufferPool[(_bufferIndex+1)%2]; }

		void AddItem(const TItem& item)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_bufferPool[_bufferIndex].emplace(item);
		}

		void SwapBuffers()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_bufferIndex ^= 1; // switch between 0 and 1
		}

	private:
		std::array<TContainer, 2> _bufferPool; // double buffer
		std::mutex _mutex;
		int _bufferIndex;
	};
}