#include "pch.h"
#include "WSA.h"

using namespace engine;

WSA::WSA()
{
	WORD wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) 
	{
		ENGINE_ERROR("Error WSAStartup [{0}]", err);
	}
}

WSA::~WSA()
{
	WSACleanup();
}
