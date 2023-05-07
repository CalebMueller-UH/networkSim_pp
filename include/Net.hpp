#pragma once

#include <unistd.h>

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "Logger.hpp"
#include "Manager.hpp"

using namespace std;

class NetLink {
 public:
  enum class LinkType { Pipe, Socket };
  NetLink(int id, int node1Id, int node2Id, LinkType type = LinkType::Pipe)
      : _id{id}, _c1{node1Id}, _c2{node2Id}, _type{type} {
    switch (_type) {
      case LinkType::Pipe:
        pipeSetup();
        break;
      case LinkType::Socket:
        socketSetup();
        break;
    }
  }

  int getId() const { return _id; }
  std::pair<int, int> getNodeIds() const { return {_c1, _c2}; }
  std::pair<int, int> getLinkFds() const { return {_c1_fd, _c2_fd}; }
  LinkType getType() const { return _type; }
  string getTypeLiteral();

 private:
  int _id{-1};
  int _c1{-1};
  int _c2{-1};
  int _c1_fd{-1};
  int _c2_fd{-1};
  LinkType _type{LinkType::Pipe};

  void pipeSetup() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
      perror("pipe");
      exit(EXIT_FAILURE);
    }
    _c1_fd = pipefd[0];
    _c2_fd = pipefd[1];
  }

  void socketSetup() {
    // Stub for socketSetup
  }
};  // End of NetLink class

//////// NETNODE BEGIN ////////
class NetNode {
 public:
  // Getters //
  enum class NodeType { Host, Switch, DNServer };
  NetNode(int id) : _id{id} {}
  NetNode(int id, NodeType type) : _id{id}, _type{type} {}

  int getId() const { return _id; }
  NodeType getType() const { return _type; }

  std::vector<NetLink*> getLinks() const {
    std::vector<NetLink*> result;
    for (const auto& link : links) {
      result.push_back(link.get());
    }
    return result;
  }

  string getTypeLiteral();

  // Setters //
  void setType(NodeType type) { _type = type; }

  // Adders //
  // from already existing link
  void addLink(const std::shared_ptr<NetLink>& link);

  // Specify arguments, and create the link to add
  void addLink(int id, int node1Id, int node2Id,
               NetLink::LinkType type = NetLink::LinkType::Pipe);

 private:
  int _id{-1};
  NodeType _type{NodeType::Host};
  std::vector<std::shared_ptr<NetLink>> links;
};  // End of NetNode class

//////// NETWORK BEGIN ////////
class Network {
 public:
  void addNode(std::unique_ptr<NetNode> node) {
    _nodes.push_back(std::move(node));
  }

  std::vector<NetNode*> getNodes() const {
    std::vector<NetNode*> result;
    for (const auto& node : _nodes) {
      result.push_back(node.get());
    }
    return result;
  }

  NetNode* getNodeById(int id) const {
    for (const auto& node : _nodes) {
      if (node->getId() == id) {
        return node.get();
      }
    }
    return nullptr;
  }

  void printNetwork() const {
    if (_nodes.size() > 0) {
      cout << color_codes[BOLD_GREEN].code;  // Set to color BOLD_GREEN
      cout << "Number of Nodes = " << numNodes << endl;
      cout << "Number of Links = " << numLinks << endl;
      for (auto& node : _nodes) {
        cout << color_codes[BOLD_GREEN].code;  // Set to color BOLD_GREEN
        cout << "Node " << node->getId() << " is a " << node->getTypeLiteral()
             << " and has " << node->getLinks().size() << " link"
             << ((node->getLinks().size() > 1) ? "s." : ".") << endl;
        for (auto& link : node->getLinks()) {
          cout << color_codes[BLUE].code;  // Set to color BLUE
          cout << "\tLink " << link->getId() << " is a "
               << link->getTypeLiteral() << " from " << link->getNodeIds().first
               << "(" << link->getLinkFds().first << ")"
               << " → " << link->getNodeIds().second << "("
               << link->getLinkFds().second << ")" << endl;
        }
      }
      cout << color_codes[BOLD_GREEN].reset;  // Reset to color default
    } else {
      cout << color_codes[RED].code << "Network is Empty"
           << color_codes[RED].reset << endl;
    }
  }

  void createAndAddLink(char c_linkType, int con1, int con2);
  void createAndAddNode(char c_nodeType, int nodeId);
  void createAndAddManagerLink(int nodeId);
  void processNodesSection(std::ifstream& configFile);
  void processLinksSection(std::ifstream& configFile);

  // netInit -- parses config file and builds the network topology
  std::vector<std::unique_ptr<NetNode>> netInit(std::string configFileName);

 private:
  std::vector<std::unique_ptr<NetNode>> _nodes;
  int numNodes;
  int numLinks;
  Manager _manager;
};  // End of Network class
