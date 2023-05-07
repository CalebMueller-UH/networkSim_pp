#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "DNSNode.hpp"
#include "Helpers.hpp"
#include "HostNode.hpp"
#include "Logger.hpp"
#include "Manager.hpp"
#include "NetLink.hpp"
#include "NetNode.hpp"
#include "PipeLink.hpp"
#include "SocketLink.hpp"
#include "SwitchNode.hpp"

using namespace std;

class Network {
 public:
  // Contructors //

  // Getters //
  std::vector<NetNode*> getNodes();
  NetNode* getNodeById(int id);

  // Adders //
  void addNode(std::unique_ptr<NetNode> node);

  void printNetwork() const;

  // netInit -- parses config file and builds the network topology
  int netInit(std::string configFileName);

 private:
  int numNodes;
  int numLinks;
  std::unique_ptr<Manager> _manager;
  std::vector<std::unique_ptr<NetNode>> _nodes;
};  // End of Network class
