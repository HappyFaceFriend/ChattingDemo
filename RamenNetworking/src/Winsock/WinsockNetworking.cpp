#include "pch.h"
#include "NetworkSystem.h"

#include <winsock.h>

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

namespace RamenNetworking
{
	NetworkSystem::Result NetworkSystem::Init()
	{
#ifdef RAMEN_ENABLE_LOGS
		Logger::Init();
#endif

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			RAMEN_LOG_ERROR("WSAStartup 2.2 failed!");
			return Result::Fail;
		}
		return Result::Success;
	}

	void NetworkSystem::Cleanup()
	{

	}
}