#pragma once

#include <Core/NetworkUtils.h>

namespace RamenNetworking
{
	class NetworkAPI
	{
	public:
		NetworkAPI() = delete;

		static Result Init();
		static void Cleanup();
	};
}