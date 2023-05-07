#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Net.hpp"

int main(int argc, char** argv) {
  Logger syslogx = Logger("syslogx");

  Network network = Network();
  network.netInit("Config/c1.config");

  // for (int i = 0; i < 10; i++) {
  //   auto node = std::make_unique<NetNode>(i);
  //   node->addLink(1, 1, 0);
  //   node->addLink(2, 1, 2);
  //   node->addLink(3, 1, 3);
  //   network.addNode(std::move(node));
  // }

  // network.printNetwork();

  return 0;
}
