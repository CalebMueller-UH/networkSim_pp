#pragma once

#include "NetNode.hpp"

class HostNode : public NetNode {
 public:
  // Constructors //
  HostNode(int id) : NetNode(id, NodeType::Host) {}

  void main() override {
    static bool isFirst = 1;
    if (isFirst) {
      cout << "HostNode" << this->getId() << " is running." << endl;
    }
  }
};  // End of HostNode class
