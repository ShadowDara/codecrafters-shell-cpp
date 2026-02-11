#pragma once

#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> Commands = { "echo", "type", "exit" };

inline bool contains(const std::vector<std::string>& v, const std::string& s);

std::vector<std::string> split(const std::string& s, char delim);
