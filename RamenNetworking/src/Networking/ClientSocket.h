#pragma once

#include <memory>

#include "Socket.h"
#include "Address.h"

#include "Core/NetworkUtils.h"

namespace RamenNetworking
{
	class ClientSocket : public virtual Socket
	{
	public:
	protected:
		ClientSocket() = default;
	public:
		virtual ~ClientSocket() = default;

		static std::unique_ptr<ClientSocket> Create();

		virtual Result Connect(const Address& serverAddress) = 0;
	};
}