#pragma once

#include <winsock.h>

#include "Networking/ServerSocket.h"
#include "WinsockSocket.h"

namespace RamenNetworking
{
	class WinsockClientSocket;

	class WinsockServerSocket : public WinsockSocket, public ServerSocket
	{
	public:
		WinsockServerSocket() = default;
		WinsockServerSocket(SOCKET nativeSocket);
		virtual ~WinsockServerSocket() = default;

		virtual Result Bind(const Address& serverAddress) override;
		virtual Result Listen(uint32_t maxQueueLength) override;
		virtual AcceptResult Accept() override;
	};
}