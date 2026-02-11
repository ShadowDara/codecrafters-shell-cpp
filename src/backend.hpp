#pragma once

#include "helper.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// Windows
#ifdef _WIN32
	#include <windows.h>
#else
	#include <cstdlib>
	#include <unistd.h>
	#include <sys/wait.h>
#endif


bool hasExtension(const std::string& cmd);

// Does not work on Windows
bool checkInPath(std::string command);

// Function to get the Absolute Path of an Executable,
// returns empty String if not found
std::string getExecutablePath(std::string command);

// Start a Process with the given Arguments, returns true if successful
// For Linux and Windows
bool runProcess(const std::vector<std::string>& args);
