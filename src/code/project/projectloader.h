#pragma once

#include <string>
#include <fstream>
#include <stdexcept>
#include "../include/json.hpp"

using json = nlohmann::json;

class ProjectLoader 
{
    public:
        explicit ProjectLoader(const std::string& configPath);
        json loadConfig(const std::string& projectType);

    private:
        std::string configPath;
};