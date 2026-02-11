#include "helper.hpp"

std::vector<std::string> Commands = { "echo", "type", "exit" };

// Helper Function to check a Vector
bool contains(const std::vector<std::string>& v, const std::string& s) {
	return std::find(v.begin(), v.end(), s) != v.end();
}


// Function to split a String by a Character
std::vector<std::string> split(const std::string& s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> result;

	while (std::getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}
