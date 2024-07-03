#include "pch.h"
#include "Networking/NetworkAPI.h"

#include <winsock.h>

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

namespace RamenNetworking
{
	Result NetworkAPI::Init()
	{
#ifdef RNET_ENABLE_LOGS
		Logger::Init();
#endif

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			RNET_LOG_ERROR("WSAStartup 2.2 failed!");
			return Result::Fail;
		}
		return Result::Success;
	}

	void NetworkAPI::Cleanup()
	{
		WSACleanup();
	}
}