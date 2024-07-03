#pragma once

#pragma warning(push)
#pragma warning(disable:4819)

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#pragma warning(pop)

namespace RamenNetworking
{
	class Logger
	{
	public:
		Logger() = delete;

	public:
		static void Init();

		constexpr static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}
#ifdef RAMEN_ENABLE_LOGS
			
	#define RAMEN_LOG(...)			::RamenNetworking::Logger::GetLogger()->trace(__VA_ARGS__)
	#define RAMEN_LOG_INFO(...)	::RamenNetworking::Logger::GetLogger()->info(__VA_ARGS__)
	#define RAMEN_LOG_WARN(...)	::RamenNetworking::Logger::GetLogger()->warn(__VA_ARGS__)
	#define RAMEN_LOG_ERROR(...)	::RamenNetworking::Logger::GetLogger()->error(__VA_ARGS__)

#else //ifdef RAMEN_ENABLE_LOGS

	#define RAMEN_LOG(...)		
	#define RAMEN_LOG_INFO(...)
	#define RAMEN_LOG_WARN(...)
	#define RAMEN_LOG_ERROR(...)

#endif //ifdef RAMEN_ENABLE_LOGS else
