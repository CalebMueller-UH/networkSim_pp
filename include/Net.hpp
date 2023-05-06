#pragma once

#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Logger.hpp"

using namespace std;

class NetNode {
 public:
  NetNode(int id) : _id{id} { cout << "Initializing " << _id << endl; }

  ~NetNode() { cout << "Deleting " << _id << endl; }

  int getId() { return _id; }

 private:
  int _id{-1};
};  // End of NetNode class

bool fileExists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

/*
    netInit -- parses config file and builds the network topology
*/
std::vector<std::unique_ptr<NetNode>> netInit(std::string configFName) {
  std::vector<std::unique_ptr<NetNode>> netNodes;

  Logger netLog = Logger("netLog");

  // Check if the file exists before attempting to open it
  if (!fileExists(configFName)) {
    std::cerr << "Error: Config file not found: " << configFName << std::endl;
    netLog.record(Logger::Priority::Error,
                  "Error: Config file not found:", configFName);
    return netNodes;
  }

  std::ifstream configFile(configFName);

  if (!configFile.is_open()) {
    std::cerr << "Error: Unable to open config file: " << configFName
              << std::endl;
    netLog.record(Logger::Priority::Error,
                  "Error: Unable to open config file:", configFName);
    return netNodes;
  }

  // std::cout << "Loading " << configFName << std::endl;

  colorPrint(BOLD_ORANGE, "Loading %s\n", configFName.c_str());

  int numNodes;
  configFile >> numNodes;
  colorPrint(GREEN, "Number of Nodes = %d\n", numNodes);

  return netNodes;
}  // End of netInit()
