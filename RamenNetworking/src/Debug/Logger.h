#pragma once

#pragma warning(push)
#pragma warning(disable:4819)

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#pragma warning(pop) // 4819

namespace RamenNetworking
{
	class Logger
	{
	public:
		Logger() = delete;

		constexpr static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static void Init();

		static std::shared_ptr<spdlog::logger> s_Logger;

#ifdef RNET_ENABLE_LOGS
		// This initializes the logger automatically using a static variable
		static struct LoggerInitializer {
			LoggerInitializer() {
				Logger::Init();
			}
		} s_Initializer;
#endif
	};

}
#ifdef RNET_ENABLE_LOGS
	#define RNET_LOG(...)			::RamenNetworking::Logger::GetLogger()->trace(__VA_ARGS__)
	#define RNET_LOG_INFO(...)	::RamenNetworking::Logger::GetLogger()->info(__VA_ARGS__)
	#define RNET_LOG_WARN(...)	::RamenNetworking::Logger::GetLogger()->warn(__VA_ARGS__)
	#define RNET_LOG_ERROR(...)	::RamenNetworking::Logger::GetLogger()->error(__VA_ARGS__)
#else //ifdef RNET_ENABLE_LOGS
	#define RNET_LOG(...)		
	#define RNET_LOG_INFO(...)
	#define RNET_LOG_WARN(...)
	#define RNET_LOG_ERROR(...)
#endif //ifdef RNET_ENABLE_LOGS else
