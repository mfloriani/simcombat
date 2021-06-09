#include "pch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace engine
{
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_appLogger;

	Log::Log()
	{
	}

	Log::~Log()
	{

	}

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("ENG");
		s_coreLogger->set_level(spdlog::level::trace);

		s_appLogger = spdlog::stdout_color_mt("APP");
		s_appLogger->set_level(spdlog::level::trace);
	}

}