#pragma once

#include "NetworkUtils.h"
#include "Networking/NetworkAPI.h"

namespace RamenNetworking
{
	class Client
	{
	public:
		Client();

		Result Init()
		{
			NetworkAPI::Init();
		}
		void Run();
	private:
	};
}