#pragma once

#include "NetLink.hpp"

class SocketLink : public NetLink {
 public:
  // Constructors //
  SocketLink(int id, int node1Id, int node2Id)
      : NetLink(id, node1Id, node2Id, LinkType::Socket) {}

  std::string recv(int recipientId, size_t maxLen) {
    // Stub
    return nullptr;
  }

  int send(int senderId, const std::string& msg) {
    // Stub
    return 0;
  }

 private:
  void connect() { return; }

};  // End of SocketLink class
