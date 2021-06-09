#pragma once

#include <memory>

#include "SocketAddress.h"

namespace engine
{
	class SocketUtil
	{
	public:
		static std::shared_ptr<SocketAddress> CreateIPv4FromString(const std::string& ip);

	};

}