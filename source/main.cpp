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

  return 0;
}
