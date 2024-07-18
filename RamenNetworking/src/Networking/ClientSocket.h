#pragma once

#include "Socket.h"
#include "Address.h"

#include "Core/NetworkUtils.h"

namespace RamenNetworking
{
	class ClientSocket : public Socket
	{
	public:
		ClientSocket();
		ClientSocket(RawSocketType rawSocket);
		virtual ~ClientSocket();

		ClientSocket(ClientSocket&& other) noexcept;
		ClientSocket& operator=(ClientSocket&& other) noexcept;

		Result Connect(const Address& serverAddress);
	};
}