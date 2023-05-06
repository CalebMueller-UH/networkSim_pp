#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"

using namespace std;

class NetNode {
 public:
  NetNode(int id) : _id{id} { cout << "Initializing " << _id << endl; }

  ~NetNode() { cout << "Deleting " << _id << endl; }

  int getId() { return _id; }

 private:
  int _id{-1};
};

/*
    netInit -- parses config file and builds the network topology
*/
std::vector<std::unique_ptr<NetNode>> netInit(std::string configFName) {
  std::vector<std::unique_ptr<NetNode>> netNodes;
  std::ifstream configFile(configFName);

  Logger netLog = Logger("netLog");

  if (!configFile.is_open()) {
    std::cerr << "Error: Unable to open config file: " << configFName
              << std::endl;
  }

  int numNodes;
  configFile >> numNodes;
  std::cout << "numNodes= " << numNodes << std::endl;

  for (int i = 0; i < numNodes; ++i) {
    std::unique_ptr<NetNode> n{new NetNode(i)};
    netNodes.push_back(std::move(n));
    netLog.record(Level::Info, "Adding", i, "to netNodes");
  }

  return netNodes;
}  // End of netInit()
