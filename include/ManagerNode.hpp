#pragma once

// #include <iostream>
// #include <memory>
// #include <ostream>
// #include <set>
// #include <string>
// #include <utility>
// #include <vector>

// #include "Color.hpp"
// #include "Logger.hpp"
// #include "Network.hpp"

#include <memory>
#include <set>

#include "NetNode.hpp"
#include "Network.hpp"

class NetLink;

//////// MANAGER BEGIN ////////
class ManagerNode : public NetNode {
 public:
  // Constructors //
  ManagerNode(int id) : NetNode(id, NodeType::Manager) {}

  void addLink(const std::shared_ptr<NetLink>& link) { _links.insert(link); }

  std::set<std::shared_ptr<NetLink>> getLinks() const { return _links; }

  void createAndAddManagerLink(int nodeId);

  void main() override {
    static bool isFirst = 1;
    if (isFirst) {
      cout << "ManagerNode" << this->getId() << " is running." << endl;
    }
  }

 private:
  std::set<std::shared_ptr<NetLink>> _links;
};  // End of ManagerNode class
