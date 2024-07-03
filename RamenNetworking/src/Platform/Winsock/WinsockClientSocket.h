#pragma once

#include <winsock.h>

#include "WinsockSocket.h"

#include "Networking/ClientSocket.h"

namespace RamenNetworking
{
	class WinsockClientSocket : public ClientSocket, public WinsockSocket
	{
	public:
		WinsockClientSocket() = default;
		WinsockClientSocket(SOCKET nativeSocket);
		virtual ~WinsockClientSocket() = default;

		virtual Result Connect(const Address& serverAddress) override;

	};
}