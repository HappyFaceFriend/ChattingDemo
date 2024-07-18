#pragma once

#include "Core/NetworkUtils.h"

namespace RamenNetworking
{
	class NetworkAPI;

	class NetworkAPIInstance
	{
	public:
		NetworkAPIInstance() = default;
		virtual ~NetworkAPIInstance() = default;

		virtual Result Init() = 0;
		virtual bool IsValid() const = 0;
	};

	class NetworkAPI
	{
	public:
		NetworkAPI() = delete;

		static Result Init() { return s_Instance->Init(); }
		static bool IsValid() { return s_Instance->IsValid(); }

	private:
		static std::unique_ptr<NetworkAPIInstance> s_Instance;
	};
}