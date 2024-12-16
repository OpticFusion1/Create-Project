#include "skeleton/loader/skeletonloader.h"

int main()
{
  SkeletonLoader loader("skeletons");
  loader.loadSkeletons();
  loader.printSkeletons();
  std::cout << "Choose a skeleton: ";
  std::string choice;
  std::cin >> choice;
  std::cin.ignore();
  Skeleton skeleton = loader.getSkeleton(choice);
  skeleton.generate();
}