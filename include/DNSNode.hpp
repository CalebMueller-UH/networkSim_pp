#pragma once

#include "NetNode.hpp"

class DNSNode : public NetNode {
 public:
  // Constructors //
  DNSNode(int id) : NetNode(id, NodeType::DNS) {}

  // Main Method Stub //
  void main() {
    // TODO: Implement main logic for DNSNode
  }
};  // End of DNSNode class
