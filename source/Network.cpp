#include "Network.hpp"

std::vector<NetNode*> Network::getNodes() {
  std::vector<NetNode*> result;
  for (const auto& node : this->_nodes) {
    result.push_back(node.get());
  }
  return result;
}  // End of Network::getNodes()

NetNode* Network::getNodeById(int id) {
  for (const auto& node : this->_nodes) {
    if (node->getId() == id) {
      return node.get();
    }
  }
  return nullptr;
}  // End of Network::getNodeById()

void Network::addNode(std::unique_ptr<NetNode> node) {
  _nodes.push_back(std::move(node));
}  // End of Network::addNode()

void Network::printNetwork() const {
  if (_nodes.size() > 0) {
    cout << color_codes[BOLD_GREEN].code;  // Set to color BOLD_GREEN
    cout << "Number of Nodes = " << numNodes << endl;
    cout << "Number of Links = " << numLinks << endl;
    for (auto& node : _nodes) {
      cout << color_codes[BOLD_GREEN].code;  // Set to color BOLD_GREEN
      cout << "Node " << node->getId() << " is a " << node->getTypeLiteral()
           << " and has " << node->getLinks().size() << " link"
           << ((node->getLinks().size() != 1) ? "s." : ".") << endl;
      for (auto& link : node->getLinks()) {
        if (link == nullptr) {
          continue;  // skip null pointers
        }
        cout << color_codes[BLUE].code;  // Set to color BLUE
        cout << "\tLink " << link->getId() << " is a " << link->getTypeLiteral()
             << " from " << link->getNodeIds().first << " → "
             << link->getNodeIds().second << endl;
      }
    }
    cout << color_codes[BOLD_GREEN].reset;  // Reset to color default
  } else {
    cout << color_codes[RED].code << "Network is Empty"
         << color_codes[RED].reset << endl;
  }
}

int Network::netInit(std::string configFileName) {
  std::vector<std::unique_ptr<NetNode>> netNodes;

  Logger netLog = Logger("netLog");

  // Check if the file exists before attempting to open it
  if (!fileExists(configFileName)) {
    std::cerr << "netInit - Config file not found: " << configFileName
              << std::endl;
    netLog.log(Logger::Priority::Error,
               "netInit - Config file not found:", configFileName);
    return 0;
  }

  std::ifstream configFile(configFileName);

  if (!configFile.is_open()) {
    std::cerr << "netInit - Unable to open config file: " << configFileName
              << std::endl;
    netLog.log(Logger::Priority::Error,
               "netInit - Unable to open config file:", configFileName);
    return 0;
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
      numNodes++;  // Count the number of nodes
      // Define variables to parse node info into
      char c_nodeType;
      int nodeId;
      std::istringstream iss(line);
      iss >> c_nodeType >> nodeId;
      std::unique_ptr<NetNode> newNode;

      // Initialize depending on node type
      switch (c_nodeType) {
        case 'H': {
          newNode = std::make_unique<HostNode>(nodeId);
          break;
        }
        case 'S': {
          newNode = std::make_unique<SwitchNode>(nodeId);
          break;
        }
        case 'D': {
          newNode = std::make_unique<DNSNode>(nodeId);
          break;
        }
      }

      addNode(std::move(newNode));

    } else if (isLinksSection) {
      numLinks++;  // Count the number of links

      // Define variables to parse link info into
      char c_linkType;
      int con1;
      int con2;
      std::istringstream iss(line);
      iss >> c_linkType >> con1 >> con2;

      std::shared_ptr<NetLink> newLink;

      // Initialize depending on link type
      switch (c_linkType) {
        case 'P': {
          newLink = std::make_shared<PipeLink>(numLinks, con1, con2);
          break;
        }
        case 'S': {
          newLink = std::make_shared<SocketLink>(numLinks, con1, con2);
          break;
        }
      }

      // Add the shared pointer of the NetLink to the Nodes matching the _id
      // values and the con1 and con2 values.
      for (auto& node : this->getNodes()) {
        if (node->getId() == con1 || node->getId() == con2) {
          node->addLink(newLink);
        }
      }
    }
  }

  return 1;
}  // End of netInit()