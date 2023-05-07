#include "NetLink.hpp"

NetLink::NetLink(int id, int node1Id, int node2Id,
                 LinkType type = LinkType::Pipe)
    : _id{id}, _end1{node1Id}, _end2{node2Id}, _type{type} {}

string NetLink::getTypeLiteral() {
  switch (_type) {
    case LinkType::Pipe:
      return "Pipe";
    case LinkType::Socket:
      return "Socket";
    default:
      return "Unknown Node Type";
  }
}  // End of NetLink::getTypeLiteral()
