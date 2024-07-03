#pragma once

namespace RamenNetworking
{
	class NetworkSystem
	{
	public:
		enum class Result { Success, Fail };

		NetworkSystem() = delete;

		static Result Init();
		static void Cleanup();
	};
}