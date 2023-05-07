#include "Net.hpp"

#include <sys/stat.h>

// Helper functions
bool fileExists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

/*
    netInit -- parses config file and builds the network topology
*/
std::vector<std::unique_ptr<NetNode>> netInit(std::string configFileName) {
  std::vector<std::unique_ptr<NetNode>> netNodes;

  Logger netLog = Logger("netLog");

  // Check if the file exists before attempting to open it
  if (!fileExists(configFileName)) {
    std::cerr << "Error: Config file not found: " << configFileName
              << std::endl;
    netLog.record(Logger::Priority::Error,
                  "Error: Config file not found:", configFileName);
    return netNodes;
  }

  std::ifstream configFile(configFileName);

  if (!configFile.is_open()) {
    std::cerr << "Error: Unable to open config file: " << configFileName
              << std::endl;
    netLog.record(Logger::Priority::Error,
                  "Error: Unable to open config file:", configFileName);
    return netNodes;
  }

  // std::cout << "Loading " << configFileName << std::endl;

  colorPrint(BOLD_ORANGE, "Loading %s\n", configFileName.c_str());

  int numNodes;
  configFile >> numNodes;
  colorPrint(GREEN, "Number of Nodes = %d\n", numNodes);

  return netNodes;
}  // End of netInit()