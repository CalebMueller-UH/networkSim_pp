#pragma once

#include <sys/stat.h>

#include <string>

// Helper function to determine if a configuration file exists
bool fileExists(const std::string& path);