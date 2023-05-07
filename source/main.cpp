#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Net.hpp"

using namespace std;

void printInstructions() {
  std::cout
      << "You must specify a configuration file when running this executable..."
      << std::endl;
  std::cout << "  Example:\t./netsim <config file name>" << std::endl;
}

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
          cout << "Host fork" << endl;
          break;
        case NetNode::NodeType::Switch:
          cout << "Switch fork" << endl;
          break;
        case NetNode::NodeType::DNServer:
          cout << "DNS fork" << endl;
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
