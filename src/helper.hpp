#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

// Available Commands in the Shell
extern std::vector<std::string> Commands;

// Function to check if a Vector contains a String
bool contains(const std::vector<std::string>& v, const std::string& s);

// Function to split a String by a Character
std::vector<std::string> split(const std::string& s, char delim);
