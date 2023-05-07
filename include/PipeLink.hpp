#pragma once

#include <unistd.h>  // For write() function

#include <iostream>
#include <stdexcept>

#include "NetLink.hpp"

class PipeLink : public NetLink {
 public:
  // Link Direction
  static const uint8_t dirRecv = 0;
  static const uint8_t dirSend = 1;

  // Constructors //
  PipeLink(int id, int node1Id, int node2Id)
      : NetLink(id, node1Id, node2Id, LinkType::Pipe) {}

  // Destructor //
  // ~PipeLink() override {}

 private:
  void connect();
  std::string recv(int recipientId, size_t maxLen);
  int send(int senderId, const std::string& msg);

  int _end1_pipefd[2];
  int _end2_pipefd[2];
};  // End of PipeLink class
