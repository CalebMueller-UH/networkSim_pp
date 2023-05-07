#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Net.hpp"

int main(int argc, char** argv) {
  Logger syslogx = Logger("syslogx");

  Network network = Network();
  network.netInit("Config/c1.config");
  network.printNetwork();

  return 0;
}
