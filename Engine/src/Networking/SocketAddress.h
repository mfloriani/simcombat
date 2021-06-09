#pragma once

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace engine
{
	class SocketAddress
	{
	public:
		SocketAddress();
		SocketAddress(unsigned int addr, int port);
		SocketAddress(const std::string& ip, int port);
		SocketAddress(const sockaddr& inSockAddr);
		
		bool operator==(const SocketAddress& other) const
		{
			return _sockAddr.sa_family == AF_INET &&
				GetSockAddrIn()->sin_port == other.GetSockAddrIn()->sin_port &&
				GetIPv4() == other.GetIPv4();
		}

		size_t GetHash() const
		{
			return GetIPv4() | GetSockAddrIn()->sin_port | _sockAddr.sa_family;
		}

		int GetSize() const { return sizeof(sockaddr); }
		
		sockaddr_in* GetSockAddrIn() { return reinterpret_cast< sockaddr_in* >( &_sockAddr ); }
		const sockaddr_in* GetSockAddrIn() const { return reinterpret_cast< const sockaddr_in* >( &_sockAddr ); }
		
		uint32_t GetIPv4() const { return static_cast<uint32_t>(GetSockAddrIn()->sin_addr.S_un.S_addr); }
				
		sockaddr* GetSockAddr() { return &_sockAddr; }

	private:
		sockaddr _sockAddr;
	};
}

namespace std
{
	template<> struct hash< engine::SocketAddress >
	{
		size_t operator()(const engine::SocketAddress& addr) const
		{
			return addr.GetHash();
		}
	};
}