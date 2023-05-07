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

class NetNode {
 public:
  enum class NodeType { Host, Switch, DNServer };
  NetNode(int id) : _id{id} { cout << "Initializing " << _id << endl; }

  ~NetNode() { cout << "Deleting " << _id << endl; }

  int getId() const { return _id; }
  NodeType getType() const { return _type; }

 private:
  int _id{-1};
  NodeType _type{NodeType::Host};
};  // End of NetNode class

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
};

class Network {
 public:
  void addNode(std::unique_ptr<NetNode> node) {
    nodes.push_back(std::move(node));
  }
  void addLink(std::unique_ptr<NetLink> link) {
    links.push_back(std::move(link));
  }

  std::vector<NetNode*> getNodes() const {
    std::vector<NetNode*> result;
    for (const auto& node : nodes) {
      result.push_back(node.get());
    }
    return result;
  }

  std::vector<NetLink*> getLinks() const {
    std::vector<NetLink*> result;
    for (const auto& link : links) {
      result.push_back(link.get());
    }
    return result;
  }

  std::vector<NetNode*> getConnectedNodes(const NetNode& node) const {
    std::vector<NetNode*> result;
    for (const auto& link : links) {
      const auto& [node1Id, node2Id] = link->getNodeIds();
      if (node1Id == node.getId()) {
        result.push_back(getNodeById(node2Id));
      } else if (node2Id == node.getId()) {
        result.push_back(getNodeById(node1Id));
      }
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

 private:
  std::vector<std::unique_ptr<NetNode>> nodes;
  std::vector<std::unique_ptr<NetLink>> links;
};

/*
    netInit -- parses config file and builds the network topology
*/
std::vector<std::unique_ptr<NetNode>> netInit(std::string configFName);
