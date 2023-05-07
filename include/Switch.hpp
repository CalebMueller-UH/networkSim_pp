#pragma once

#include "NetNode.hpp"

class Switch : public NetNode {
 public:
  // Constructors //
  Switch(int id) : NetNode(id, NodeType::Switch) {}

  // Main Method Stub //
  void main() {
    // TODO: Implement main logic for Switch
  }
};  // End of Switch class
