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
	#include <direct.h>
#else
	#include <cstdlib>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <fcntl.h>
#endif


bool hasExtension(const std::string& cmd);

// Does not work on Windows
bool checkInPath(std::string command);

// Function to get the Absolute Path of an Executable,
// returns empty String if not found
std::string getExecutablePath(std::string command);

// Start a Process with the given Arguments, returns true if successful
// For Linux and Windows
bool runProcess(const std::vector<std::string>& args,
	bool redirect = false,
	bool redirectStderr = false,
	const std::string& filename = "");

// Function to change the current Directory,
// returns true if successful
bool changeDirectory(const std::string& path);

// Change to the Home Directory, returns true if successful
bool changeToHome();
