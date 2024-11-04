#pragma once

#include <filesystem>
#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
#endif

class Util
{
    public:
        std::filesystem::path getRelativeFilePath(const std::string& relativePath);

        std::filesystem::path getExecutablePath();
};