#include "util.h"


std::filesystem::path Util::getRelativeFilePath(const std::string& relativePath) 
{
    return getExecutablePath() / relativePath;
}

std::filesystem::path Util::getExecutablePath() 
{
    #if defined(_WIN32) || defined(_WIN64)
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return std::filesystem::path(buffer).parent_path();
    #elif defined(__linux__)
        char buffer[1024];
        ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
        return std::filesystem::path(std::string(buffer, (count > 0) ? count : 0)).parent_path();
    #elif defined(__APPLE__)
        char buffer[1024];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0) {
            return std::filesystem::path(buffer).parent_path();
        } 
        else 
        {
            throw std::runtime_error("Buffer too small; should not happen");
        }
    #else
        throw std::runtime_error("Unsupported platform");
    #endif
}