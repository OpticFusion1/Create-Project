#include "cli.h"
#include "../util/util.h"
#include <unistd.h>

namespace fs = std::filesystem;

CLI::CLI(ProjectLoader& loader) : loader(loader) {}

std::string str_tolower(std::string str)
{
	transform(str.begin(), str.end(), str.begin(), [](unsigned char c){return tolower(c);});
	return str;
}

std::string replace_all(std::string str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) 
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }

    return str;
}

void setup_git(std::string path)
{
	std::cout << "[INFO] Setting up git repo\n" << std::endl;

	if (chdir(path.c_str()) != 0)
	{
		perror("[ERROR] Unable to change directory");
		return;
	}

	if (system("git init") == -1)
	{
		perror("[ERROR] Unable to initialize git");
		return;
	}

	if (system("git add .") == -1)
	{
		perror("[Error] Unable to 'git add .'");
		return;
	}

	if (system("git commit -m \"Initial Commit\"") == -1)
	{
		perror("[ERROR] Unable to make initial commit");
		return;
	}

	std::cout << "[INFO] Created initial commit" << std::endl;
}

void CLI::start() 
{
    json config = loader.loadConfig("project_types");
    std::vector<std::string> projectTypes;

    for (const auto& type : config["types"]) 
    {
        projectTypes.push_back(type.get<std::string>());
    }

    std::cout << "Available project types:\n";
    for (size_t i = 0; i < projectTypes.size(); ++i)
    {
        std::cout << (i + 1) << ". " << projectTypes[i] << '\n';
    }
    std::cout << "Choose a project type: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice < 1 || choice > static_cast<int>(projectTypes.size())) 
    {
        throw std::runtime_error("Invalid choice.");
    }

    std::string projectType = projectTypes[choice - 1];
    std::cout << "Project Type: " << projectType << std::endl;

    json projectConfig = loader.loadConfig(projectType);

    std::unordered_map<std::string, std::string> userInputs;
    for (const auto& field : projectConfig["fields"]) 
    {
        std::cout << "Enter " << field["prompt"].get<std::string>() << ": ";
        std::string value;
        std::getline(std::cin, value);
        userInputs[field["name"]] = value;
    }

    generateProject(projectConfig, userInputs);
}

void CLI::generateProject(const json& config, const std::unordered_map<std::string, std::string>& userInputs) {
    try 
    {
        if (userInputs.find("projectName") == userInputs.end())
        {
            throw std::runtime_error("Missing 'projectName' in user inputs.");
        }
        bool dirsOnly = false;
        bool setupGit = true;
        if (config.contains("config") && config["config"].is_object())
        {
            const auto& configObj = config["config"];
            setupGit = configObj.contains("setup_git") && configObj["setup_git"].is_boolean() ? configObj["setup_git"].get<bool>() : setupGit;
            dirsOnly = configObj.contains("directories_only") && configObj["directories_only"].is_boolean() ? configObj["directories_only"].get<bool>() : dirsOnly;
        }
        std::string outputDir = replace_all(str_tolower(userInputs.at("projectName")), " ", "_");
        std::cout << "Creating project at: " << outputDir << std::endl;
        auto createDirectories = [&](const std::string& path) {
            fs::create_directories(fs::path(TemplateRenderer::renderTemplate(path, userInputs)));
        };
        if (!dirsOnly) 
        {
            fs::create_directories(outputDir);
        }

        if (config.contains("directories")) 
        {
            if (!config["directories"].is_array()) 
            {
                throw std::runtime_error("'directories' configuration key is not an array");
            }
            
            for (const auto& dir : config["directories"]) 
            {
                if (!dir.contains("path")) 
                {
                    throw std::runtime_error("Each directory object must contain a 'path' key.");
                }
                if (dirsOnly) 
                {
                    createDirectories(dir["path"].get<std::string>());
                } 
                else 
                {
                    std::string fullFilePath = outputDir + "/" + dir["path"].get<std::string>();
                    createDirectories(fullFilePath);
                }
            }
        }

        if (!dirsOnly) 
        {
            if (!config.contains("files") || !config["files"].is_array())
            {
                throw std::runtime_error("Missing or invalid 'files' key in configuration.");
            }
            for (const auto& file : config["files"]) 
            {
                if (!file.contains("path") || !file.contains("template")) 
                {
                    throw std::runtime_error("Each file object must contain 'path' and 'template' keys.");
                }
                std::string relativeFilePath = TemplateRenderer::renderTemplate(file["path"].get<std::string>(), userInputs);
                fs::path fullFilePath = fs::path(outputDir) / relativeFilePath;
                fs::create_directories(fullFilePath.parent_path());
                std::string templatePath = file["template"].get<std::string>();
                std::string fileContent = loadTemplateContent(templatePath, userInputs);
                std::ofstream outFile(fullFilePath);
                if (!outFile) 
                {
                    throw std::runtime_error("Could not open file for writing: " + fullFilePath.string());
                }
                outFile << fileContent;
                outFile.close();
                std::cout << "Generated file: " << fullFilePath.string() << std::endl;
            }
        }
        if (setupGit) 
        {
            setup_git(outputDir);
        }
        std::cout << "Project created successfully at " << outputDir << std::endl;
    } catch (const std::exception& e) 
    {
        std::cerr << "Error in CLI::generateProject: " << e.what() << std::endl;
    }
}

std::string CLI::loadTemplateContent(const std::string& templatePath, const std::unordered_map<std::string, std::string>& userInputs)
 {
    Util util;
    std::ifstream templateFile(util.getRelativeFilePath(templatePath));
    if (!templateFile) 
    {
        throw std::runtime_error("Could not open template file: " + templatePath);
    }
    std::string content((std::istreambuf_iterator<char>(templateFile)), std::istreambuf_iterator<char>());
    templateFile.close();
    return TemplateRenderer::renderTemplate(content, userInputs);
}