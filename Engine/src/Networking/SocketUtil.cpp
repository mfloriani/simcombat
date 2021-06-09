#include "pch.h"
#include "SocketUtil.h"
#include "Constants.h"
#include "Config.h"

using namespace engine;

std::shared_ptr<SocketAddress> SocketUtil::CreateIPv4FromString(const std::string& addr)
{
	auto pos = addr.find_last_of(':');
	std::string host, service;
	if (pos != std::string::npos) // has ip and port?
	{
		host = addr.substr(0, pos);
		service = addr.substr(pos + 1);
	}
	else // only ip
	{
		host = addr;
		int port = std::stoi(Config::Get().GetConfigValue("server_port"));
		service = std::to_string(port);
	}

	addrinfo hint;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;

	addrinfo* result;
	int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
	if (error != 0 && result != nullptr)
	{
		ENGINE_ERROR("Error creating IPv4 from string {0}", addr);
		return nullptr;
	}

	while (!result->ai_addr && result->ai_next)
	{
		result = result->ai_next;
	}

	if (!result->ai_addr)
	{
		return nullptr;
	}

	auto socketAddr = std::make_shared<SocketAddress>(*result->ai_addr);

	freeaddrinfo(result);

	return socketAddr;
}
