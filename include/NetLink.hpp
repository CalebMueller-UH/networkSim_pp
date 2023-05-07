#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "Helpers.hpp"
#include "Logger.hpp"

class NetLink {
 public:
  // Log
  Logger _log{Logger("linklog", 1024 * 1024)};

  // Enum Type //
  enum class LinkType { Pipe, Socket };

  // Constructors //
  NetLink(int id, int node1Id, int node2Id, LinkType type);

  // Virtual Destructor //
  // virtual ~NetLink() {}

  // Getters //
  int getId() const { return _id; }
  int get_end1() const { return _end1; }
  int get_end2() const { return _end2; }
  std::pair<int, int> getNodeIds() const { return {_end1, _end2}; }
  LinkType getType() const { return _type; }
  string getTypeLiteral();

  //  Sender, Receiver made virtual for   //
  //  different link type implementations //
  std::string recv(int recipientId, size_t maxLen);
  int send(int senderId, const std::string& msg);

 private:
  // Setup Functions
  void connect();

 private:
  int _id{-1};
  int _end1{-1};
  int _end2{-1};

  LinkType _type{LinkType::Pipe};
};  // End of NetLink class