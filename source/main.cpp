#include <iostream>

#include "Logger.hpp"
#include "Net.hpp"

int main(int argc, char** argv) {
  Logger syslogx = Logger("syslogx");

  std::vector<std::unique_ptr<NetNode>> nodes = netInit("Config/c1.config");

  for (int i = 0;; i++) {
    syslogx.record(i);
  }

  return 0;
}
