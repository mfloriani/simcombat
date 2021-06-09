#include "pch.h"
#include "SocketAddress.h"

using namespace engine;

SocketAddress::SocketAddress()
{
	GetSockAddrIn()->sin_family = AF_INET;
	GetSockAddrIn()->sin_addr.S_un.S_addr = INADDR_ANY;
	GetSockAddrIn()->sin_port = 0;
}

SocketAddress::SocketAddress(unsigned int addr, int port)
{
	GetSockAddrIn()->sin_family = AF_INET;
	GetSockAddrIn()->sin_addr.S_un.S_addr = htonl(addr);
	GetSockAddrIn()->sin_port = htons(port);
}

SocketAddress::SocketAddress(const std::string& ip, int port)
{
	GetSockAddrIn()->sin_family = AF_INET;
	GetSockAddrIn()->sin_port = htons(port);
	inet_pton(GetSockAddrIn()->sin_family, ip.c_str(), &GetSockAddrIn()->sin_addr);
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr)
{
	memcpy(&_sockAddr, &inSockAddr, sizeof(sockaddr));
}
