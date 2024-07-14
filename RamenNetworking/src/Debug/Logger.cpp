#include "pch.h"
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace RamenNetworking
{
#ifdef RNET_ENABLE_LOGS
	Logger::LoggerInitializer Logger::s_Initializer;
#endif // ifdef RNET_ENABLE_LOGS
	std::shared_ptr<spdlog::logger> Logger::s_Logger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_Logger = spdlog::stdout_color_mt("Networking");
		s_Logger->set_level(spdlog::level::trace);
	}

}