#include "pch.h"
#include "NetworkAPI.h"

#ifdef RNET_PLATFORM_WINDOWS
	#include "Platform/Winsock/WinsockNetworkAPI.h"
#endif

namespace RamenNetworking
{
	std::unique_ptr<NetworkAPIInstance> NetworkAPI::s_Instance = std::make_unique<WinsockNetworkAPI>();
}