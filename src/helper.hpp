#pragma once

#include <string>
#include <sstream>
#include <vector>

extern std::vector<std::string> Commands;

bool contains(const std::vector<std::string>& v, const std::string& s);

std::vector<std::string> split(const std::string& s, char delim);
