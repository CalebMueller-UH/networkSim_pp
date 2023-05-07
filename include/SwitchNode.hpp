#pragma once

#include "NetNode.hpp"

class SwitchNode : public NetNode {
 public:
  // Constructors //
  SwitchNode(int id) : NetNode(id, NodeType::Switch) {}

  // Main Method Stub //
  void main() {
    // TODO: Implement main logic for SwitchNode
  }
};  // End of SwitchNode class
