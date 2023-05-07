#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Net.hpp"

int main(int argc, char** argv) {
  Logger syslogx = Logger("syslogx");

  Network network = Network();
  auto node1 = std::make_unique<NetNode>(1);
  auto node2 = std::make_unique<NetNode>(2);
  auto link1 = std::make_unique<NetLink>(1, 1, 2);
  auto link2 = std::make_unique<NetLink>(2, 2, 4);
  network.addNode(std::move(node1));
  network.addNode(std::move(node2));
  network.addLink(std::move(link1));
  network.addLink(std::move(link2));

  auto nodes = network.getNodes();
  auto links = network.getLinks();

  for (const auto& node : nodes) {
    cout << "Node " << node->getId() << " has "
         << network.getConnectedNodes(*node).size() << " connected nodes."
         << endl;
  }

  cout << "Network has " << links.size() << " link"
       << (links.size() != 1 ? "s" : "") << endl;

  for (const auto& link : links) {
    cout << "Link " << link->getId() << " between " << link->getNodeIds().first
         << " and " << link->getNodeIds().second << endl;
  }

  return 0;
}
