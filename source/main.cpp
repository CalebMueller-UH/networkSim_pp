#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Net.hpp"

int main(int argc, char** argv) {
  Logger syslogx = Logger("syslogx");

  std::vector<std::unique_ptr<NetNode>> nodes;

  if (argc > 1) {
    string configFName = "Config/" + string(argv[1]);
    cout << configFName << endl;
    nodes = netInit(configFName);

  } else {
    cout << "Usage: " << argv[0] << " <config_file>" << endl;
    return 1;
  }

  return 0;
}
