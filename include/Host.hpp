#pragma once

#include "NetNode.hpp"

class Host : public NetNode {
 public:
  // Constructors //
  Host(int id) : NetNode(id, NodeType::Host) {}

  // Main Method Stub //
  void main() {
    // TODO: Implement main logic for Host
  }
};  // End of Host class
