#include "pch.h"
#include "NetworkAPI.h"

#ifdef RNET_PLATFORM_WINDOWS
	#include "Platform/Winsock/WinsockNetworkAPI.h"
#endif

namespace RamenNetworking
{
	std::unique_ptr<NetworkAPI> NetworkAPI::Create()
	{
#ifdef RNET_PLATFORM_WINDOWS
		return std::make_unique<WinsockNetworkAPI>();
#endif
		RNET_ASSERT(false, "Current platform is not supported.");
		return nullptr;
	}

}