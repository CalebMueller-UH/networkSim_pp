#include "Net.hpp"

#include <sys/stat.h>

#include "Common.hpp"

///////////////////////////////
//////// NETLINK BEGIN ////////
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

///////////////////////////////
//////// NETNODE BEGIN ////////
string NetNode::getTypeLiteral() {
  switch (_type) {
    case NodeType::Host:
      return "Host";
    case NodeType::Switch:
      return "Switch";
    case NodeType::DNServer:
      return "DNServer";
    default:
      return "Unknown Node Type";
  }
}  // End of NetNode::getTypeLiteral()

void NetNode::addLink(const std::shared_ptr<NetLink>& link) {
  links.push_back(link);
}  // End of NetNode::addLink()

void NetNode::addLink(int id, int node1Id, int node2Id,
                      NetLink::LinkType type) {
  std::shared_ptr<NetLink> link(new NetLink(id, node1Id, node2Id));
  links.push_back(std::move(link));
}  // End of NetNode::addLink(int, int, int, type)

///////////////////////////////
//////// NETWORK BEGIN ////////

void Network::processNodesSection(std::ifstream& configFile) {
  std::string line;
  while (std::getline(configFile, line) && line != "Links:") {
    if (line.empty()) continue;
    char c_nodeType;
    int nodeId;
    std::istringstream iss(line);
    iss >> c_nodeType >> nodeId;
    createAndAddNode(c_nodeType, nodeId);
  }
}

void Network::processLinksSection(std::ifstream& configFile) {
  std::string line;
  while (std::getline(configFile, line)) {
    if (line.empty()) continue;
    char c_linkType;
    int con1;
    int con2;
    std::istringstream iss(line);
    iss >> c_linkType >> con1 >> con2;
    createAndAddLink(c_linkType, con1, con2);
  }
}

void Network::createAndAddNode(char c_nodeType, int nodeId) {
  numNodes++;
  std::unique_ptr<NetNode> newNode;
  switch (c_nodeType) {
    case 'H':
      newNode = std::make_unique<NetNode>(nodeId, NetNode::NodeType::Host);
      createAndAddManagerLink(nodeId);
      break;
    case 'S':
      newNode = std::make_unique<NetNode>(nodeId, NetNode::NodeType::Switch);
      break;
    case 'D':
      newNode = std::make_unique<NetNode>(nodeId, NetNode::NodeType::DNServer);
      break;
  }
  addNode(std::move(newNode));
}

void Network::createAndAddLink(char c_linkType, int con1, int con2) {
  numLinks++;
  auto n1 = getNodeById(con1);
  auto n2 = getNodeById(con2);
  std::shared_ptr<NetLink> link;
  switch (c_linkType) {
    case 'P':
      link = std::make_shared<NetLink>(numLinks, con1, con2,
                                       NetLink::LinkType::Pipe);
      break;
    case 'S':
      link = std::make_shared<NetLink>(numLinks, con1, con2,
                                       NetLink::LinkType::Socket);
      break;
  }
  if (n1) n1->addLink(link);
  if (n2) n2->addLink(link);
}

void Network::createAndAddManagerLink(int nodeId) {
  auto managerLink =
      std::make_shared<NetLink>(numLinks, -1, nodeId, NetLink::LinkType::Pipe);
  numLinks++;
  this->_manager.addLink(managerLink);
  auto newNode = getNodeById(nodeId);
  if (newNode) newNode->addLink(managerLink);
}

std::vector<std::unique_ptr<NetNode>> Network::netInit(
    std::string configFileName) {
  std::vector<std::unique_ptr<NetNode>> netNodes;

  Logger netLog = Logger("netLog");

  // Check if the file exists before attempting to open it
  if (!fileExists(configFileName)) {
    std::cerr << "netInit - Config file not found: " << configFileName
              << std::endl;
    netLog.log(Logger::Priority::Error,
               "netInit - Config file not found:", configFileName);
    return netNodes;
  }

  std::ifstream configFile(configFileName);

  if (!configFile.is_open()) {
    std::cerr << "Error: Unable to open config file: " << configFileName
              << std::endl;
    netLog.log(Logger::Priority::Error,
               "Error: Unable to open config file:", configFileName);
    return netNodes;
  }

  colorPrint(BOLD_ORANGE, "Loading %s\n", configFileName.c_str());

  std::string line;
  numNodes = 0;
  numLinks = 0;
  bool isNodesSection = false;
  bool isLinksSection = false;

  while (std::getline(configFile, line)) {
    if (line.empty()) {
      continue;
    }

    if (line == "Nodes:") {
      isNodesSection = true;
      continue;
    } else if (line == "Links:") {
      isNodesSection = false;
      isLinksSection = true;
      continue;
    }

    if (isNodesSection) {
      char c_nodeType;
      int nodeId;
      std::istringstream iss(line);
      iss >> c_nodeType >> nodeId;
      numNodes++;
      std::unique_ptr<NetNode> newNode;
      switch (c_nodeType) {
        case 'H': {
          newNode = std::make_unique<NetNode>(nodeId, NetNode::NodeType::Host);

          // Create a link between the Manager and the Host node
          auto managerLink = std::make_shared<NetLink>(numLinks, -1, nodeId,
                                                       NetLink::LinkType::Pipe);
          numLinks++;

          // Add the link to the Manager and the Host node
          this->_manager.addLink(managerLink);
          newNode->addLink(managerLink);

          break;
        }
        case 'S': {
          newNode =
              std::make_unique<NetNode>(nodeId, NetNode::NodeType::Switch);
          break;
        }
        case 'D': {
          newNode =
              std::make_unique<NetNode>(nodeId, NetNode::NodeType::DNServer);
          break;
        }
      }

      addNode(std::move(newNode));
    } else if (isLinksSection) {  // Links section
      char c_linkType;
      int con1;
      int con2;
      std::istringstream iss(line);
      iss >> c_linkType >> con1 >> con2;
      numLinks++;

      // Find the nodes with the corresponding IDs
      auto n1 = getNodeById(con1);
      auto n2 = getNodeById(con2);

      // Create the link and add it to each node
      switch (c_linkType) {
        case 'P': {
          auto link = std::make_shared<NetLink>(numLinks, con1, con2,
                                                NetLink::LinkType::Pipe);
          if (n1) {
            n1->addLink(link);
          }
          if (n2) {
            n2->addLink(link);
          }
          break;
        }
        case 'S': {
          auto link = std::make_shared<NetLink>(numLinks, con1, con2,
                                                NetLink::LinkType::Socket);
          if (n1) {
            n1->addLink(link);
          }
          if (n2) {
            n2->addLink(link);
          }
          break;
        }
      }
    }
  }

  return netNodes;
}  // End of netInit()