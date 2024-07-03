#pragma once

#include <memory>

#include "Socket.h"
#include "Address.h"

#include "core/NetworkUtils.h"

namespace RamenNetworking
{
	class ClientSocket : public virtual Socket
	{
	public:
		static std::unique_ptr<ClientSocket> Create();

		virtual ~ClientSocket() = default;

		virtual Result Connect(const Address& serverAddress) = 0;

	protected:
		ClientSocket() = default;
	};
}