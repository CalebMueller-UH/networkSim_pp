#include "Net.hpp"

#include <sys/stat.h>

//////// NETLINK BEGIN ////////
// No functions to add here

//////// NETNODE BEGIN ////////

//////// NETWORK BEGIN ////////

// Helper function to determine if a configuration file exists
bool fileExists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

// netInit -- parses config file and builds the network topology
std::vector<std::unique_ptr<NetNode>> Network::netInit(
    std::string configFileName) {
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

  colorPrint(BOLD_ORANGE, "Loading %s\n", configFileName.c_str());

  std::string line;
  int numNodes = 0;
  int numLinks = 0;
  bool isNodesSection = false;
  bool isLinksSection = false;

  while (std::getline(configFile, line)) {
    if (line.empty()) {
      continue;
    }

    if (line == "Nodes:") {
      isNodesSection = true;
      continue;
    } else if (line == "Links:") {
      isNodesSection = false;
      isLinksSection = true;
      continue;
    }

    char nodeType;
    int nodeId;
    std::istringstream iss(line);
    iss >> nodeType >> nodeId;

    if (isNodesSection) {
      numNodes++;

      switch (nodeType) {
        case 'H':
          // Add code for initializing H type NetNode
          break;
        case 'S':
          // Add code for initializing S type NetNode
          break;
        case 'D':
          // Add code for initializing D type NetNode
          break;
      }
    } else if (isLinksSection) {  // Links section
      numLinks++;

      switch (nodeType) {
        case 'P':
          // Add code for handling P type link
          break;
        case 'S':
          // Add code for handling S type link
          break;
      }
    }
  }

  colorPrint(GREEN, "Number of Nodes = %d\n", numNodes);
  colorPrint(GREEN, "Number of Links = %d\n", numLinks);

  return netNodes;
}  // End of netInit()
