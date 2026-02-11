// Implementation of a Shell in C++

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "helper.hpp"
#include "backend.hpp"


// Would be cool to add
// ls
// mkdir


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

		// View the current Directory
		else if (words[0] == "pwd")
		{
			std::cout << fs::current_path().string() << "\n";
		}

		// Change Directory
		else if (words[0] == "cd")
		{
			if (words[1] == "~")
			{
				changeToHome();
			}

			else {
				changeDirectory(words[1]);
			}
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
				std::cout << words[1] << " is " 
					<< getExecutablePath(words[1]) << "\n";
			}

			// Others
			else
			{
				std::cout << words[1] << ": not found\n";
			}
		}

		// Executable found for that
		else if (checkInPath(words[0]))
		{
			// Run the Executable
			runProcess(words);
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
