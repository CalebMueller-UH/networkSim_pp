#include "Net.hpp"

#include <sys/stat.h>

//////// NETLINK BEGIN ////////
// No functions to add here

//////// NETNODE BEGIN ////////

//////// NETWORK BEGIN ////////

// Helper function to determine if a configuration file exists
bool fileExists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

// netInit -- parses config file and builds the network topology
std::vector<std::unique_ptr<NetNode>> Network::netInit(
    std::string configFileName) {
  std::vector<std::unique_ptr<NetNode>> netNodes;

  Logger netLog = Logger("netLog");

  // Check if the file exists before attempting to open it
  if (!fileExists(configFileName)) {
    std::cerr << "Error: Config file not found: " << configFileName
              << std::endl;
    netLog.log(Logger::Priority::Error,
               "Error: Config file not found:", configFileName);
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
        case 'H':
          newNode = std::make_unique<NetNode>(nodeId, NetNode::NodeType::Host);
          break;
        case 'S':
          newNode =
              std::make_unique<NetNode>(nodeId, NetNode::NodeType::Switch);
          break;
        case 'D':
          newNode =
              std::make_unique<NetNode>(nodeId, NetNode::NodeType::DNServer);
          break;
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
        case 'P':
          if (n1) {
            n1->addLink(NetLink(numLinks, con1, con2, NetLink::LinkType::Pipe));
          }
          if (n2) {
            n2->addLink(NetLink(numLinks, con1, con2, NetLink::LinkType::Pipe));
          }
          break;
        case 'S':
          if (n1) {
            n1->addLink(
                NetLink(numLinks, con1, con2, NetLink::LinkType::Socket));
          }
          if (n2) {
            n2->addLink(
                NetLink(numLinks, con1, con2, NetLink::LinkType::Socket));
          }
      }
    }
  }

  return netNodes;
}  // End of netInit()