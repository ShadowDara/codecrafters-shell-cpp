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
#endif


bool hasExtension(const std::string& cmd);

bool checkInPath(std::string command);

//static std::string getExecutablePath(std::string command);
