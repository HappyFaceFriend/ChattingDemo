#include "pch.h"
#include "WinsockNetworkAPI.h"

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

namespace RamenNetworking
{
	WinsockNetworkAPI::WinsockNetworkAPI()
	{
		if (WSAStartup(MAKEWORD(2, 2), &m_WSAData) != 0)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("WSAStartup 2.2 failed! WSAErrorCode: {0}", errorCode);
			m_WSAStatus = WSAStatus::InitFailed;
		}
		else
		{
			RNET_LOG("Win Socket API 2.2 Initialized.");
			m_WSAStatus = WSAStatus::InitSuccessed;
		}
	}

	WinsockNetworkAPI::~WinsockNetworkAPI()
	{
		WSACleanup();
	}
}