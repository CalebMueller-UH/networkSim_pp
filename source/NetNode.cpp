#include "NetNode.hpp"

std::vector<NetLink*> NetNode::getLinks() const {
  std::vector<NetLink*> result;
  for (const auto& link : links) {
    result.push_back(link.get());
  }
  return result;
}

NetNode::NodeType NetNode::getType() const { return this->_type; }

string NetNode::getTypeLiteral() {
  switch (_type) {
    case NodeType::Host:
      return "Host";
    case NodeType::Switch:
      return "Switch";
    case NodeType::DNS:
      return "DNS";
    default:
      return "Unknown Node Type";
  }
}  // End of NetNode::getTypeLiteral()

void NetNode::addLink(const std::shared_ptr<NetLink>& link) {
  links.push_back(link);
}  // End of NetNode::addLink()
