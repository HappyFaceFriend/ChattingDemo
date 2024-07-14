#pragma once

#include <Core/NetworkUtils.h>

namespace RamenNetworking
{
	class NetworkAPI
	{
	public:
		NetworkAPI() = default;
		virtual ~NetworkAPI() = default;

		static std::unique_ptr<NetworkAPI> NetworkAPI::Create();

		virtual bool IsValid() const = 0;
	};
}