#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Network.hpp"

using namespace std;

void printInstructions();  // Forward Declaration

int main(int argc, char** argv) {
  Logger syslog = Logger("syslog");

  if (argc <= 1) {
    printInstructions();
    return -1;
  }

  Network network = Network();
  network.netInit(argv[1]);
  network.printNetwork();

  for (auto& node : network.getNodes()) {
    int pid = 0;
    pid = fork();
    if (pid == 0) {
      // Child Process
      switch (node->getType()) {
        case NetNode::NodeType::Host:
          node->main();
          break;
        case NetNode::NodeType::Switch:
          node->main();
          break;
        case NetNode::NodeType::DNS:
          node->main();
          break;
      }
      cout.flush();  // flush the buffer
      exit(0);       // terminate child process after executing its task
    } else {
      // Parent Process
    }
  }

  return 0;
}

void printInstructions() {
  std::cout
      << "You must specify a configuration file when running this executable..."
      << std::endl;
  std::cout << "  Example:\t./netsim <config file name>" << std::endl;
}
