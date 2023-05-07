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
#include "Helpers.hpp"
#include "Logger.hpp"
#include "NetLink.hpp"

//////// NETNODE BEGIN ////////
class NetNode {
 public:
  // Type Enum //
  enum class NodeType { Host, Switch, DNS };

  // Constructors //
  NetNode(int id) : _id{id} {}
  NetNode(int id, NodeType type) : _id{id}, _type{type} {}

  // Getters //
  int getId() const { return _id; }
  std::vector<NetLink*> getLinks() const;
  NodeType getType() const;
  string getTypeLiteral();

  // Adders //
  // from already existing link
  void addLink(const std::shared_ptr<NetLink>& link);

  virtual void main() {}

 private:
  int _id{-1};
  NodeType _type{NodeType::Host};
  std::vector<std::shared_ptr<NetLink>> links;
};  // End of NetNode class