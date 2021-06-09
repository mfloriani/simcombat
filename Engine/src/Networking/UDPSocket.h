#pragma once

#include "SocketAddress.h"

namespace engine
{
	class UDPSocket
	{
	public:
		UDPSocket();
		UDPSocket(SOCKET s) : _socket(s) {};
		~UDPSocket();

		bool Bind(SocketAddress& address);
		int Send(const char* data, int size, SocketAddress& addr);
		int Receive(char* data, int size, SocketAddress& addr);
		bool SetNonBlockingMode(bool nonBlocking);

	private:
		SOCKET _socket;
	};
}