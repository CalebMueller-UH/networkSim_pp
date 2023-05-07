#pragma once

#include "NetNode.hpp"

class DNServer : public NetNode {
 public:
  // Constructors //
  DNServer(int id) : NetNode(id, NodeType::DNServer) {}

  // Main Method Stub //
  void main() {
    // TODO: Implement main logic for DNServer
  }
};  // End of DNServer class
