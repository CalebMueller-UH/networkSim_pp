#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "Logger.hpp"

using namespace std;

//////// NETLINK BEGIN ////////
class NetLink {
 public:
  enum class LinkType { Pipe, Socket };
  NetLink(int id, int node1Id, int node2Id, LinkType type = LinkType::Pipe)
      : _id{id}, _c1{node1Id}, _c2{node2Id}, _type{type} {}

  int getId() const { return _id; }
  LinkType getType() const { return _type; }
  std::pair<int, int> getNodeIds() const { return {_c1, _c2}; }
  string getTypeLiteral();

 private:
  int _id{-1};
  int _c1{-1};
  int _c2{-1};
  int _c1_fd{-1};
  int _c2_fd{-1};
  LinkType _type{LinkType::Pipe};
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
  void addLink(const NetLink& link);

  // Specify arguments, and create the link to add
  void addLink(int id, int node1Id, int node2Id,
               NetLink::LinkType type = NetLink::LinkType::Pipe);

 private:
  int _id{-1};
  NodeType _type{NodeType::Host};
  std::vector<std::unique_ptr<NetLink>> links;
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
        cout << "\tLink " << link->getId() << " is a " << link->getTypeLiteral()
             << " from " << link->getNodeIds().first << " → "
             << link->getNodeIds().second << endl;
      }
    }
    cout << color_codes[BOLD_GREEN].reset;  // Reset to color default
  }

  // netInit -- parses config file and builds the network topology
  std::vector<std::unique_ptr<NetNode>> netInit(std::string configFileName);

 private:
  std::vector<std::unique_ptr<NetNode>> _nodes;
  int numNodes;
  int numLinks;
};  // End of Network class
