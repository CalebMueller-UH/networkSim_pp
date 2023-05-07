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
  NetLink(int id, int node1Id, int node2Id)
      : _id{id}, _node1Id{node1Id}, _node2Id{node2Id} {}

  int getId() const { return _id; }
  LinkType getType() const { return _type; }
  std::pair<int, int> getNodeIds() const { return {_node1Id, _node2Id}; }

 private:
  int _id{-1};
  int _node1Id{-1};
  int _node2Id{-1};
  LinkType _type{LinkType::Pipe};
};  // End of NetLink class

//////// NETNODE BEGIN ////////
class NetNode {
 public:
  // Getters //
  enum class NodeType { Host, Switch, DNServer };
  NetNode(int id) : _id{id} {}

  int getId() const { return _id; }
  NodeType getType() const { return _type; }

  std::vector<NetLink*> getLinks() const {
    std::vector<NetLink*> result;
    for (const auto& link : links) {
      result.push_back(link.get());
    }
    return result;
  }

  // Setters //
  void setType(NodeType type) { _type = type; }

  // Adders //
  // from already existing link
  void addLink(const NetLink& link) {
    std::unique_ptr<NetLink> copiedLink(new NetLink(
        link.getId(), link.getNodeIds().first, link.getNodeIds().second));
    links.push_back(std::move(copiedLink));
  }

  // Specify arguments, and create the link to add
  void addLink(int id, int node1Id, int node2Id) {
    std::unique_ptr<NetLink> link(new NetLink(id, node1Id, node2Id));
    links.push_back(std::move(link));
  }

 private:
  int _id{-1};
  NodeType _type{NodeType::Host};
  std::vector<std::unique_ptr<NetLink>> links;
};  // End of NetNode class

//////// NETWORK BEGIN ////////
class Network {
 public:
  void addNode(std::unique_ptr<NetNode> node) {
    nodes.push_back(std::move(node));
  }

  std::vector<NetNode*> getNodes() const {
    std::vector<NetNode*> result;
    for (const auto& node : nodes) {
      result.push_back(node.get());
    }
    return result;
  }

  NetNode* getNodeById(int id) const {
    for (const auto& node : nodes) {
      if (node->getId() == id) {
        return node.get();
      }
    }
    return nullptr;
  }

  void printNetwork() const {
    cout << color_codes[BOLD_GREEN].code;
    for (auto& node : nodes) {
      cout << "Node " << node->getId() << " has " << node->getLinks().size()
           << " links." << endl;
      for (auto& link : node->getLinks()) {
        cout << "\tLink " << link->getId() << ": from "
             << link->getNodeIds().first << " → " << link->getNodeIds().second
             << endl;
      }
    }
    cout << color_codes[BOLD_GREEN].reset;
  }

  // netInit -- parses config file and builds the network topology
  std::vector<std::unique_ptr<NetNode>> netInit(std::string configFileName);

 private:
  std::vector<std::unique_ptr<NetNode>> nodes;
};  // End of Network class
