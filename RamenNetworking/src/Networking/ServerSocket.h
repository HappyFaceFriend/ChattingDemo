#pragma once

#include <memory>

#include "Socket.h"
#include "Address.h"
#include "ClientSocket.h"

#include "core/NetworkUtils.h"

namespace RamenNetworking
{
	class ServerSocket : public Socket
	{
	public:
		struct AcceptResult
		{
			// TODO : Find a way to not heap allocate this
			std::unique_ptr<ClientSocket> ClientSocket;
			Address ClientAddress;
		};

		ServerSocket();
		ServerSocket(RawSocketType rawSocket);
		virtual ~ServerSocket();

		Result Listen(uint32_t maxQueueLength);
		Result Bind(const Address& serverAddress);
		AcceptResult Accept();
	};
}
