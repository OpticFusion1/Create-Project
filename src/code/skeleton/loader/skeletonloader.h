#pragma once

#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include "../skeleton.h"

namespace fs = std::filesystem;

class SkeletonLoader
{
  public:
    SkeletonLoader(const std::string& skeletonsPath);

    void loadSkeletons();

    Skeleton getSkeleton(const std::string& name);

    void printSkeletons();
  private:
    std::string skeletonsPath;
    std::unordered_map<std::string, Skeleton> skeletons;
};