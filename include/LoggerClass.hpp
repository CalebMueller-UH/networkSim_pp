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
  Logger(string instanceName, size_t maxFileSize = 1024 * 1024) {
    this->instanceName = instanceName;
    this->maxFileSize = maxFileSize;
    logFile_path = "Logs/" + instanceName + ".log";
    openLogFile();
    minLogLevel = Priority::Debug;
  }

  ~Logger() { logFile.close(); }

  void setMinLogLevel(Priority minLogLevel) { this->minLogLevel = minLogLevel; }

  int record(string input, Priority priority) {
    if (priority < minLogLevel) {
      return 0;
    }

    logFile << "[" << currentDateTime() << "] ";

    switch (priority) {
      case Priority::Debug:
        logFile << "[DEBUG] ";
        break;
      case Priority::Info:
        logFile << "[INFO] ";
        break;
      case Priority::Warning:
        logFile << "[WARNING] ";
        break;
      case Priority::Error:
        logFile << "[ERROR] ";
        break;
      default:
        break;
    }

    logFile << input << endl;
    return 0;
  }

  template <typename... Args>
  int formattedRecord(string format, Priority priority, Args... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
    unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return record(string(buf.get(), buf.get() + size - 1), priority);
  }

  template <typename... Args>
  int conditionalRecord(bool condition, string input, Priority priority,
                        Args... args) {
    if (condition) {
      return formattedRecord(input, priority, args...);
    }
    return 0;
  }

  int clear() {
    if (!logFile.is_open()) {
      throw runtime_error("Log file is not open");
    }

    logFile.close();
    logFile.open(logFile_path, ios::out | ios::trunc);
    if (!logFile.is_open()) {
      throw runtime_error("Could not clear log file " + logFile_path);
    }
    return 0;
  }

  void flush() { logFile.flush(); }

  // Implement log rotation based on file size
  void checkAndRotateLog() {
    logFile.flush();
    if (getFileSize(logFile_path) > maxFileSize) {
      logFile.close();
      rotateLog();
      openLogFile();
    }
  }

 private:
  ofstream logFile;
  string logFile_path;
  string instanceName;
  Priority minLogLevel;
  size_t maxFileSize;

  string currentDateTime() {
    auto now = chrono::system_clock::now();
    auto now_time_t = chrono::system_clock::to_time_t(now);
    auto now_tm = localtime(&now_time_t);
    stringstream ss;
    ss << put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
  }

  void openLogFile() {
    logFile.open(logFile_path, ios::out | ios::app);
    if (!logFile.is_open()) {
      throw runtime_error("Could not create log file " + logFile_path);
    }
  }

  size_t getFileSize(const string &filePath) {
    struct stat stat_buf;
    int rc = stat(filePath.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
  }

  void rotateLog() {
    int i = 1;
    string newPath;
    do {
      newPath = "Logs/" + instanceName + "_" + to_string(i) + ".log";
      i++;
    } while (getFileSize(newPath) != static_cast<size_t>(-1));
    rename(logFile_path.c_str(), newPath.c_str());
  }
};
