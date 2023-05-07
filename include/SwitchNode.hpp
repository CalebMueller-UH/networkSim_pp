#pragma once

#include "NetNode.hpp"

class SwitchNode : public NetNode {
 public:
  // Constructors //
  SwitchNode(int id) : NetNode(id, NodeType::Switch) {}

  void main() override {
    static bool isFirst = 1;
    if (isFirst) {
      cout << "SwitchNode" << this->getId() << " is running." << endl;
    }
  }
};  // End of SwitchNode class
