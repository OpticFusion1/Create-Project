#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <regex>

class TemplateRenderer
{
    public:
        static std::string renderTemplate(const std::string& templatePath, const std::unordered_map<std::string, std::string>& variables);
};