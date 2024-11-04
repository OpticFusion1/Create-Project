#include "projectloader.h"
#include "../util/util.h"

ProjectLoader::ProjectLoader(const std::string& configPath) : configPath(configPath) {}

json ProjectLoader::loadConfig(const std::string& projectType) 
{
    Util util;
    std::ifstream configFile(util.getRelativeFilePath(configPath + projectType + ".json").string());
    json config;
    if (configFile.is_open()) 
    {
        configFile >> config;
        configFile.close();
    } else 
    {
        std::cout << util.getRelativeFilePath(configPath + projectType + ".json").string() << std::endl;
        throw std::runtime_error("Could not open configuration file.");
    }
    return config;
}