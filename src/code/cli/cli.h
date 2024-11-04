#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../project/projectloader.h"
#include "../template/templaterenderer.h"

class CLI 
{
public:
    explicit CLI(ProjectLoader& loader);
    void start();

private:
    ProjectLoader& loader;
    std::string getProjectType(int choice);
    void generateProject(const json& config, const std::unordered_map<std::string, std::string>& userInputs);
    std::string loadTemplateContent(const std::string& templatePath, const std::unordered_map<std::string, std::string>& userInputs);
};
