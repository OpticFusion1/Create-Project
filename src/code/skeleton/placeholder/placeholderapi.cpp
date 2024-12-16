#include "placeholderapi.h"
#include <fstream>
#include <stdexcept>
#include <sstream>

std::string PlaceholderAPI::translatePlaceholders(const std::string& templateStr, const std::unordered_map<std::string, std::string>& userInputs) 
{
    std::string result = templateStr;

    for (const auto& [key, value] : userInputs) 
    {
        std::string placeholder = "\\{" + key + "\\}";
        result = std::regex_replace(result, std::regex(placeholder), value);
    }
    return result;
}