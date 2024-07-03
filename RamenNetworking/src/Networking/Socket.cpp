#include "pch.h"
#include "ServerSocket.h"
#include "ClientSocket.h"


#ifdef RNET_PLATFORM_WINDOWS
#include "Platform/Winsock/WinsockClientSocket.h"
#include "Platform/Winsock/WinsockServerSocket.h"
#endif

namespace RamenNetworking
{
	std::unique_ptr<ClientSocket> ClientSocket::Create()
	{
#ifdef RNET_PLATFORM_WINDOWS
		return std::make_unique<WinsockClientSocket>();
#endif
		RNET_ASSERT(false, "Current platform is not supported.");
		return nullptr;
	}

	std::unique_ptr<ServerSocket> ServerSocket::Create()
	{
#ifdef RNET_PLATFORM_WINDOWS
		return std::make_unique<WinsockServerSocket>();
#endif
		RNET_ASSERT(false, "Current platform is not supported.");
		return nullptr;
	}
}