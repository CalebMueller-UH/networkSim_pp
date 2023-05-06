#pragma once

#include <sys/stat.h>

#include <chrono>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

class Logger {
 public:
  enum class Priority { Debug, Info, Warning, Error };

  // Default logger maxFileSize set to 1MB
  Logger(string instanceName, size_t maxFileSize = 1024 * 1024);
  ~Logger() { logFile.close(); }

  void setMinLogLevel(Priority minLogLevel);
  void setDefaultLogLevel(Priority defaultLogLevel);
  int clear();
  void flush() { logFile.flush(); }
  void checkAndRotateLog();

  // Template function for logging formatted strings or arbitrary variable types
  template <typename... Args>
  int record(Priority priority, Args... args) {
    if (priority < minLogLevel) {
      return 0;
    }
    stringstream ss;
    ss << "▹[" << currentDateTime() << "] ";
    switch (priority) {
      case Priority::Debug:
        ss << "[DEBUG] ";
        break;
      case Priority::Info:
        ss << "[INFO] ";
        break;
      case Priority::Warning:
        ss << "[WARNING] ";
        break;
      case Priority::Error:
        ss << "[ERROR] ";
        break;
      default:
        break;
    }
    ((ss << args << " "),
     ...);       // add arguments to stringstream with a space separator
    ss << endl;  // add newline character to stringstream
    logFile << ss.str();  // write stringstream contents to log file
    checkAndRotateLog();
    return 1;
  }

  // Template function that handles arguments given with no explicit priority
  template <typename... Args>
  int record(Args... args) {
    return record(defaultLogLevel, args...);
  }

  template <typename... Args>
  int conditionalRecord(bool condition, string input, Priority priority,
                        Args... args) {
    if (condition) {
      return record(priority, input, args...);
    }
    return 0;
  }

 private:
  ofstream logFile;

  string logFile_path;
  string instanceName;

  Priority minLogLevel;
  Priority defaultLogLevel = Priority::Debug;

  // max file size in bits
  size_t maxFileSize;

  string currentDateTime();
  void openLogFile();
  size_t getFileSize(const string &filePath);
  void rotateLog();
};

using Level = Logger::Priority;