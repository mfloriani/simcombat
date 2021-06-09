#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace engine
{
	class Log
	{
	public:
		Log();
		~Log();

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_appLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_appLogger;
	};
}

#define ENGINE_TRACE(...)		::engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)		::engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)		::engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)		::engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...)	::engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define GAME_TRACE(...)			::engine::Log::GetAppLogger()->trace(__VA_ARGS__)
#define GAME_INFO(...)			::engine::Log::GetAppLogger()->info(__VA_ARGS__)
#define GAME_WARN(...)			::engine::Log::GetAppLogger()->warn(__VA_ARGS__)
#define GAME_ERROR(...)			::engine::Log::GetAppLogger()->error(__VA_ARGS__)
#define GAME_CRITICAL(...)		::engine::Log::GetAppLogger()->critical(__VA_ARGS__)

