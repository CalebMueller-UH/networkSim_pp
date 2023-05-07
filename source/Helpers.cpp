#include "Helpers.hpp"

// Helper function to determine if a configuration file exists
bool fileExists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}