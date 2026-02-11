// Implementation of a Shell in C++

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <windows.h>
#include <filesystem>
#include <windows.h>
namespace fs = std::filesystem;


std::vector<std::string> Commands = { "echo", "type", "exit" };


// Helper Function to check a Vector
static inline bool contains(const std::vector<std::string>& v, const std::string& s) {
	return std::find(v.begin(), v.end(), s) != v.end();
}


// Function to split a String by a Character
static std::vector<std::string> split(const std::string& s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> result;

	while (std::getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}



// To check if a Function is available in the Path Var
static bool checkInPath(std::string command)
{
	// PATH holen
	char buffer[32767];
	DWORD len = GetEnvironmentVariableA("PATH", buffer, sizeof(buffer));
	if (len == 0) {
		//std::cerr << "PATH not found\n";
		return false;
	}

	std::string path_env(buffer);
	auto paths = split(path_env, ';');

	// PATHEXT holen (z.B. .EXE;.BAT;.CMD)
	DWORD ext_len = GetEnvironmentVariableA("PATHEXT", buffer, sizeof(buffer));
	if (ext_len == 0) {
		return false;
	}
	std::string pathext_env(buffer, ext_len);

	auto exts = split(pathext_env, ';');

	for (const auto& dir : paths) {

		for (const auto& ext : exts) {

			fs::path full_path = fs::path(dir) / (command + ext);

			if (fs::exists(full_path) && fs::is_regular_file(full_path)) {
				std::cout << command << " is "
					<< fs::absolute(full_path) << "\n";
				return true;
			}
		}
	}
	return false;
}


// Shell Function
int shell() {
	// Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	// TODO: Uncomment the code below to pass the first stage
	while (true)
	{
		std::cout << "$ ";

		std::string input;
		std::getline(std::cin, input);

		std::stringstream ss(input);
		std::string word;
		std::vector<std::string> words;

		while (ss >> word) {   // trennt automatisch nach whitespace
			words.push_back(word);
		}

		/*for (const auto& w : words) {
			std::cout << w << std::endl;
		}*/

		// Leave the Shell
		if (words[0] == "exit")
		{
			break;
		}

		// Echo Command
		else if (words[0] == "echo")
		{
			for (size_t i = 1; i < words.size(); ++i)
			{
				std::cout << words[i];
				if (i < words.size() - 1)
				{
					std::cout << " ";
				}
			}
			std::cout << "\n";
		}

		// Type Command
		else if (words[0] == "type")
		{
			// Check for builtin Commands
			if (contains(Commands, words[1]))
			{
				std::cout << words[1] << " is a shell builtin\n";
			}

			else if (checkInPath(words[1]))
			{
				std::cout << "lol";
			}

			// Others
			else
			{
				std::cout << words[1] << ": not found\n";
			}
		}

		// Default
		else
		{
			std::cout << input << ": command not found\n";
		}
	}

	return 0;
}


// Main Function
int main(int argc, char* argv[])
{
	return shell();
}
