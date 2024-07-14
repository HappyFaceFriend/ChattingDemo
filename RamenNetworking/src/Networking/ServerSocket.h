#pragma once

#include <memory>

#include "Socket.h"
#include "Address.h"

#include "core/NetworkUtils.h"

namespace RamenNetworking
{
	class ClientSocket;

	class ServerSocket : public virtual Socket
	{
	public:
		struct AcceptResult
		{
			std::unique_ptr<ClientSocket> ClientSocket;
			Address ClientAddress;
		};

	protected:
		ServerSocket() = default;
	public:
		virtual ~ServerSocket() = default;

		static std::unique_ptr<ServerSocket> Create();

		virtual Result Listen(uint32_t maxQueueLength) = 0;
		virtual Result Bind(const Address& serverAddress) = 0;
		virtual AcceptResult Accept() = 0;
	};
}
