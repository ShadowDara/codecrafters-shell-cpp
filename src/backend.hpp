#pragma once

#include "helper.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Windows
#ifdef _WIN32
	#include <windows.h>
#else
	#include <cstdlib>
	#include <unistd.h>
#endif

bool checkInPath(std::string command);

//static std::string getExecutablePath(std::string command);
