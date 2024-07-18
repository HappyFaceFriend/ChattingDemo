#pragma once

#include <memory>

#include "Socket.h"
#include "Address.h"
#include "ClientSocket.h"

#include "Core/NetworkUtils.h"

namespace RamenNetworking
{
	class ServerSocket : public Socket
	{
	public:
		struct AcceptResult
		{
			ClientSocket clientSocket;
			Address clientAddress;
		};

		ServerSocket();
		ServerSocket(RawSocketType rawSocket);
		virtual ~ServerSocket();

		Result Listen(uint32_t maxQueueLength);
		Result Bind(const Address& serverAddress);
		AcceptResult Accept();
	};
}
