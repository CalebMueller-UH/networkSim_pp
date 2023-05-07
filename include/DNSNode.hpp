#pragma once

#include "NetNode.hpp"

class DNSNode : public NetNode {
 public:
  // Constructors //
  DNSNode(int id) : NetNode(id, NodeType::DNS) {}

  void main() override {
    static bool isFirst = 1;
    if (isFirst) {
      cout << "DNSNode" << this->getId() << " is running." << endl;
    }
  }
};  // End of DNSNode class
