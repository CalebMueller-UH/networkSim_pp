#pragma once

#include "NetNode.hpp"

class HostNode : public NetNode {
 public:
  // Constructors //
  HostNode(int id) : NetNode(id, NodeType::Host) {}

  // Main Method Stub //
  void main() {
    // TODO: Implement main logic for HostNode
  }
};  // End of HostNode class
