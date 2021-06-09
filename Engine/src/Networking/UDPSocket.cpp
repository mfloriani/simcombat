#include "pch.h"
#include "UDPSocket.h"

using namespace engine;

UDPSocket::UDPSocket() : _socket(0)
{
	_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (_socket == INVALID_SOCKET)
	{
		ENGINE_ERROR("Error creating UDP socket {0}", WSAGetLastError());
	}
}

UDPSocket::~UDPSocket()
{
	closesocket(_socket);
}

bool UDPSocket::Bind(SocketAddress& address)
{
	int err = bind(_socket, address.GetSockAddr(), address.GetSize());
	if (err != 0)
	{
		ENGINE_ERROR("Error binding UDP socket {0}", WSAGetLastError());
		return false;
	}
	return true;
}

int UDPSocket::Send(const char* data, int size, SocketAddress& addr)
{
	int bytes = sendto(_socket, data, size, 0, addr.GetSockAddr(), addr.GetSize());
	if (bytes <= 0)
	{
		int error = WSAGetLastError();
		if (error == WSAEFAULT)
		{
			ENGINE_ERROR("Error sending UDP data [{0}: Bad Address]", error);
		}
		else
		{
			ENGINE_ERROR("Error sending UDP data {0}", error);
		}
		return -error;
	}
	return bytes;
}

int UDPSocket::Receive(char* data, int size, SocketAddress& addr)
{
	socklen_t fromLength = addr.GetSize();	
	int bytes = recvfrom(_socket, data, size, 0, addr.GetSockAddr(), &fromLength);

	if (bytes >= 0)
	{
		return bytes;
	}
	else
	{
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			// non-blocking socket raise this when no data queued
			return 0;
		}
		else if (error == WSAECONNRESET)
		{
			char ip[INET_ADDRSTRLEN];
			int len = INET_ADDRSTRLEN;
			WSAAddressToString(addr.GetSockAddr(), addr.GetSize(), NULL, (LPWSTR)&ip, (LPDWORD)&len);
			ENGINE_ERROR("Error receiving UDP data [{0}:WSAECONNRESET]! Connection reset from {1}:{2}", error, ip, addr.GetSockAddrIn()->sin_port);
			return -WSAECONNRESET;
		}
		else
		{
			ENGINE_ERROR("Error receiving UDP data {0}", error);
			return -error;
		}
	}
}

bool UDPSocket::SetNonBlockingMode(bool nonBlocking)
{	
	u_long arg = nonBlocking ? 1 : 0;
	int result = ioctlsocket(_socket, FIONBIO, &arg);

	if (result == SOCKET_ERROR)
	{
		ENGINE_ERROR("Error setting non blocking socket {0}", WSAGetLastError());
		return false;
	}
	
	return true;
}
