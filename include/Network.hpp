#pragma once

#include <string>
#include <vector>

#include "DNSNode.hpp"
#include "HostNode.hpp"
#include "ManagerNode.hpp"
#include "NetLink.hpp"
#include "NetNode.hpp"
#include "PipeLink.hpp"
#include "SocketLink.hpp"
#include "SwitchNode.hpp"

class ManagerNode;  // Forward declaration

class Network {
 public:
  // Constructors //

  // Getters //
  std::vector<NetNode*> getNodes();
  NetNode* getNodeById(int id);

  // Adders //
  void addNode(std::unique_ptr<NetNode> node);
  void addManager(std::unique_ptr<ManagerNode> node);

  void printNetwork() const;

  // netInit -- parses config file and builds the network topology
  int netInit(std::string configFileName);

 public:
  std::unique_ptr<ManagerNode> _manager;

 private:
  int numNodes;
  int numLinks;
  std::vector<std::unique_ptr<NetNode>> _nodes;
};  // End of Network class
