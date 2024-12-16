#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <regex>

class PlaceholderAPI
{
    public:
        static std::string translatePlaceholders(const std::string& templatePath, const std::unordered_map<std::string, std::string>& variables);
};