#include <iostream>

#include "LoggerClass.hpp"

int main() {
  Logger *syslogx = new Logger("syslogx");

  for (int i = 0; i < 1000000; i++) {
    syslogx->record("Test1", Logger::Priority::Debug);
    syslogx->record("Test2", Logger::Priority::Info);
    syslogx->record("Test3", Logger::Priority::Warning);
    syslogx->record("Test4", Logger::Priority::Error);
  }

  // syslogx->clear();

  return 0;
}
