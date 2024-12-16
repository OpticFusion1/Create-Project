#pragma once

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include "../include/json.hpp"
#include "../util/util.h"
#include "placeholder/placeholderapi.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

class Skeleton
{
  public:
    Skeleton(const std::string skeletonDir);

    std::string getSkeletonDir();

    void load();

    void generate();

  private:
    std::string skeletonDir;
    std::vector<json> fields;
    std::vector<json> directories;
    std::vector<json> files;
    std::vector<json> configOptions;
};