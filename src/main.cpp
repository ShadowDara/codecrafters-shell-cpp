#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]) {
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

		// Default
		else
		{
			std::cout << input << ": command not found\n";
		}
	}
}
