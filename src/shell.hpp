#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "helper.hpp"
#include "backend.hpp"


namespace Shell
{
	std::vector<std::string> parseLine(std::string line);

	int run();
}
