#pragma once

#include <string>
#include <unordered_map>

namespace engine
{
	class Config
	{
	public:
		~Config() = default;

		static Config& Get()
		{
			static Config instance;
			return instance;
		}

		bool LoadConfigFile(const std::string& filename);
		std::string GetConfigValue(const std::string& key);

	private:
		Config() {}

	private:
		std::unordered_map<std::string, std::string> _configMap;
	};
}