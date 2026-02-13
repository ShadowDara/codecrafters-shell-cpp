#include "shell.hpp"


// Function to parse the Input Line
std::vector<std::string> Shell::parseLine(std::string line)
{
	std::vector<std::string> tokens;

	//while (ss >> word) {   // trennt automatisch nach whitespace
	//	tokens.push_back(word);
	//}

	// Change:
	// Parse by character

	std::string word = "";
	char lastChar = '0';

	// Double Quotes are more important than single Quotes
	bool doubleQuote = false;
	bool firstQuote = false;

	for (size_t i = 0; i < line.size(); i++)
	{
		// ESCAPE
		if (!doubleQuote && !firstQuote && line[i] == '\\')
		{
			// Skip the next Character
			if (i + 1 < line.size())
			{
				word += line[i + 1];
				i++;
			}
			continue;
		}

		// DOUBLE QUOTE
		if (line[i] == '"' && !firstQuote)
		{
			doubleQuote = !doubleQuote;
			continue;
		}

		// SINGLE QUOTE
		if (line[i] == '\'' && !doubleQuote)
		{
			firstQuote = !firstQuote;
			continue;
		}

		// SPACE OUTSIDE QUOTES
		if (!doubleQuote && !firstQuote && line[i] == ' ')
		{
			if (!word.empty())
			{
				tokens.push_back(word);
				word.clear();
			}
			continue;
		}

		// NORMAL CHAR
		word += line[i];
	}

	// Add the Last Word ofc
	// letztes Wort
	if (!word.empty())
	{
		tokens.push_back(word);
	}

	return tokens;
}


// Shell Function
int Shell::run() {
	// Flush after every std::cout / std:cerr
	//std::cout << std::unitbuf;
	//std::cerr << std::unitbuf;

	// TODO: Uncomment the code below to pass the first stage
	while (true)
	{
		std::cout << "$ ";

		std::string input;
		std::getline(std::cin, input);

		std::vector<std::string> words;

		words = parseLine(input);

		/*for (const auto& w : words) {
			std::cout << w << std::endl;
		}*/

		//
		// TODO
		// Parser for " and '
		//

		// Leave the Shell
		if (words[0] == "exit")
		{
			break;
		}

		// Echo Command
		else if (words[0] == "echo")
		{
			std::string output = "";

			for (size_t i = 1; i < words.size(); i++)
			{
				output += words[i] + " ";
			}

			output += "\n";

			std::cout << output;
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
