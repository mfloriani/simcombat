#include "pch.h"
#include "Config.h"

using namespace engine;

bool Config::LoadConfigFile(const std::string& filename)
{
	char tmpPath[MAX_PATH];
	GetModuleFileNameA(NULL, tmpPath, MAX_PATH);
	std::string path(tmpPath);
	path = path.substr(0, 1 + path.find_last_of('\\'));
	path += filename;
	ENGINE_TRACE("Loading config file [{0}]", path);

	std::ifstream fin(path);
	if (!fin)
	{
		ENGINE_ERROR("Error opening config file {0}", filename);
		return false;
	}

	std::string key, value;
	while (fin >> key)
	{
		fin >> value;
		_configMap.insert(std::make_pair(key, value));
	}

	return true;
}

std::string Config::GetConfigValue(const std::string& key)
{
	auto it = _configMap.find(key);
	if (it != _configMap.end())
		return it->second;

	ENGINE_ERROR("Error to load config value {0}", key);
	return std::string();
}
