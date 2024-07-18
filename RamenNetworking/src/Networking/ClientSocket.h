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

		Result Connect(const Address& serverAddress);
	};
}