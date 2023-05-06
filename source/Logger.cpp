#include "Logger.hpp"

// Default logger maxFileSize set to 1MB
Logger::Logger(string instanceName, size_t maxFileSize) {
  this->instanceName = instanceName;
  this->maxFileSize = maxFileSize;
  this->setDefaultLogLevel(Priority::Warning);
  logFile_path = "Logs/" + instanceName + ".log";
  openLogFile();
  minLogLevel = Priority::Debug;
}

void Logger::setMinLogLevel(Priority minLogLevel) {
  this->minLogLevel = minLogLevel;
}

void Logger::setDefaultLogLevel(Priority defaultLogLevel) {
  this->defaultLogLevel = defaultLogLevel;
}

int Logger::clear() {
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

void Logger::checkAndRotateLog() {
  logFile.flush();
  if (getFileSize(logFile_path) > maxFileSize) {
    logFile.close();
    // Truncate the file to clear its content instead of rotating
    logFile.open(logFile_path, ios::out | ios::trunc);
    if (!logFile.is_open()) {
      throw runtime_error("Unable to open log file for truncation: " +
                          logFile_path);
    }
  }
}

string Logger::currentDateTime() {
  auto now = chrono::system_clock::now();
  auto now_time_t = chrono::system_clock::to_time_t(now);
  auto now_tm = localtime(&now_time_t);
  stringstream ss;
  ss << put_time(now_tm, "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

void Logger::openLogFile() {
  logFile.open(logFile_path, ios::out | ios::app);
  if (!logFile.is_open()) {
    throw runtime_error("Could not create log file " + logFile_path);
  }
}

size_t Logger::getFileSize(const string &filePath) {
  struct stat stat_buf;
  int rc = stat(filePath.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : -1;
}

void Logger::rotateLog() {
  int i = 1;
  string newPath;
  do {
    newPath = "Logs/" + instanceName + "_" + to_string(i) + ".log";
    i++;
  } while (getFileSize(newPath) != static_cast<size_t>(-1));
  rename(logFile_path.c_str(), newPath.c_str());
}
