#include "skeletonloader.h"

SkeletonLoader::SkeletonLoader(const std::string& skeletonsPath) : skeletonsPath(skeletonsPath) {}

void SkeletonLoader::loadSkeletons()
{
  try 
  {
    for (const auto& entry : fs::directory_iterator(skeletonsPath))
    {
      std::string skeletonDir = entry.path().filename().string();
      Skeleton skeleton(skeletonDir);
      skeleton.load();  
      std::pair<std::string, Skeleton> pair(skeletonDir, skeleton);
      skeletons.insert(pair);
    }
  } catch (const fs::filesystem_error& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

Skeleton SkeletonLoader::getSkeleton(const std::string& name)
{
  auto it = skeletons.find(name);
  if (it == skeletons.end())
  {
    // TODO: Implement proper error handling
    throw std::runtime_error("The skeleton '" + name + "' does not exist");
  }
  return it->second;
}

void SkeletonLoader::printSkeletons()
{
  std::cout << "Available Skeletons:" << std::endl;
  for (const auto& [key, skeleton] : skeletons)
  {
    std::cout << "- " << key << std::endl;
  }
}