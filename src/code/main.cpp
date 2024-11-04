#include "project/projectloader.h"
#include "cli/cli.h"
#include <iostream>

int main() 
{
    try 
    {
        ProjectLoader loader("configs/");
        CLI cli(loader);
        cli.start();
    } catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}