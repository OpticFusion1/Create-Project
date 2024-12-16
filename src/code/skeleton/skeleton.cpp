#include "skeleton.h"

Skeleton::Skeleton(const std::string skeletonDir) : skeletonDir(skeletonDir) {}

template <typename T>
void loadJsonArray(const nlohmann::json& config, const std::string& key, std::vector<T>& container) {
  // FIXME: Throws an error for certain missing things.
  if (!config.contains(key) || !config[key].is_array())
  {
    std::cerr << "Key '" + key + "' is missing or is not an array." << std::endl;
    return;
  }
  for (const auto& item : config[key])
  {
    container.push_back(item);
  }
}

void Skeleton::load()
{
  std::cout << "Loading skeleton " << skeletonDir << std::endl;
  std::string configFileLoc = "skeletons/" + skeletonDir + "/config.json";
  std::ifstream configFile(configFileLoc);
  if (!configFile.is_open())
  {
    throw std::runtime_error("Could not open configuration file '" + configFileLoc + "'.");
  }
  json config;
  configFile >> config;
  configFile.close();
  loadJsonArray(config, "fields", fields);
  loadJsonArray(config, "directories", directories);
  loadJsonArray(config, "files", files);
  loadJsonArray(config, "config", configOptions);
  std::cout << "Loaded skeleton " << skeletonDir << std::endl;
}

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
  try {
      fs::current_path(path);
  } catch (const fs::filesystem_error& e) {
      std::cerr << "Error changing directory: " << e.what() << std::endl;
  }
  // Requires git to be runnable via system, but it's better than increasing the size even more with a library
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

std::string loadTemplateContent(const std::string& templatePath, const std::unordered_map<std::string, std::string>& userInputs)
{
  Util util;
  std::ifstream templateFile(util.getRelativeFilePath(templatePath));
  if (!templateFile) 
  {
    throw std::runtime_error("Could not open template file: " + templatePath);
  }
  std::string content((std::istreambuf_iterator<char>(templateFile)), std::istreambuf_iterator<char>());
  templateFile.close();
  PlaceholderAPI placeholderAPI;
  return placeholderAPI.translatePlaceholders(content, userInputs);
}

void Skeleton::generate()
{
  std::unordered_map<std::string, std::string> userInputs;
  for (const auto& field : fields)
  {
    std::cout << "Enter " << field["prompt"].get<std::string>() << ": ";
    std::string value;
    std::getline(std::cin, value);
    userInputs[field["name"]] = value;
  }
  if (userInputs.find("projectName") == userInputs.end())
  {
    throw std::runtime_error("Missing 'projectName' in user inputs.");
  }
  bool dirsOnly = false;
  bool setupGit = true;
  auto findOption = [this](const std::string& key) {
    return std::find_if(configOptions.begin(), configOptions.end(),[&key](const auto& jsonObj) { return jsonObj.contains(key); });
  };
  auto itSetupGit = findOption("setup_git");
  if (itSetupGit != configOptions.end() && (*itSetupGit)["setup_git"].is_boolean()) {
    setupGit = (*itSetupGit)["setup_git"].get<bool>();
  }
  auto itDirsOnly = findOption("directories_only");
  if (itDirsOnly != configOptions.end() && (*itDirsOnly)["directories_only"].is_boolean()) {
    dirsOnly = (*itDirsOnly)["directories_only"].get<bool>();
  }
  std::string outputDir = replace_all(str_tolower(userInputs.at("projectName")), " ", "_");
  std::cout << "Creating project at: " << outputDir << std::endl;
  PlaceholderAPI placeholderAPI;
  auto createDirectories = [&](const std::string& path)
  {
    fs::create_directories(fs::path(placeholderAPI.translatePlaceholders(path, userInputs)));
  };
  if (!dirsOnly)
  {
    fs::create_directories(outputDir);
  }
  for (const auto& dir : directories)
  {
    if (!dir.contains("path"))
    {
      throw std::runtime_error("Each directory object must contain a 'path' key.");
    }
    if (dirsOnly)
    {
      createDirectories(dir["path"].get<std::string>());
    } else
    {
      std::string fullFilePath = outputDir + "/" + dir["path"].get<std::string>();
      createDirectories(fullFilePath);
    }
  }
  if (!dirsOnly)
  {
    for (const auto& file : files)
    {
      if (!file.contains("path") || !file.contains("template"))
      {
        throw std::runtime_error("Each file object must contain 'path' and 'template' keys.");
      }
      PlaceholderAPI placeholderAPI;
      std::string relativeFilePath = placeholderAPI.translatePlaceholders(file["path"].get<std::string>(), userInputs);
      fs::path fullFilePath = fs::path(outputDir) / relativeFilePath;
      fs::create_directories(fullFilePath.parent_path());
      std::string templatePath = file["template"].get<std::string>();
      std::string fileContent = loadTemplateContent("skeletons/" + skeletonDir + "/files/" + templatePath, userInputs);
      std::ofstream outFile(fullFilePath);
      if (!outFile) 
      {
        throw std::runtime_error("Could not open file for writing: " + fullFilePath.string());
      }
      outFile << fileContent;
      outFile.close();
      std::cout << "Generated file: " << fullFilePath.string() << std::endl;
    }
    if (setupGit) 
    {
        setup_git(outputDir);
    }
    std::cout << "Project created successfully at " << outputDir << std::endl;
  }
}

std::string Skeleton::getSkeletonDir()
{
  return skeletonDir;
}