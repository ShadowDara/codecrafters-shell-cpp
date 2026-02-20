#include "shell.hpp"

namespace fs = std::filesystem;

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
	bool singleQuote = false;

	for (size_t i = 0; i < line.size(); i++)
	{
		// ESCAPE
		if (line[i] == '\\')
		{
			if (doubleQuote)
			{
				// in double quotes: nur bestimmte chars escapen
				if (i + 1 < line.size() &&
					(
						line[i + 1] == '"' ||
						line[i + 1] == '\\' ||
						line[i + 1] == '$' ||
						line[i + 1] == '`'
					))
				{
					word += line[i + 1];
					i++;
					continue;
				}
			}
			else if (!singleQuote)
			{
				// outside quotes: escape everything
				if (i + 1 < line.size())
				{
					word += line[i + 1];
					i++;
					continue;
				}
			}

			// sonst: literal backslash
			word += '\\';
			continue;
		}

		// DOUBLE QUOTE
		if (line[i] == '"' && !singleQuote)
		{
			doubleQuote = !doubleQuote;
			continue;
		}

		// SINGLE QUOTE
		if (line[i] == '\'' && !doubleQuote)
		{
			singleQuote = !singleQuote;
			continue;
		}

		// SPACE OUTSIDE QUOTES
		if (!doubleQuote && !singleQuote && line[i] == ' ')
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
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	// TODO: Uncomment the code below to pass the first stage
	while (true)
	{
		std::cout << "$ ";

		std::string input;
		std::getline(std::cin, input);

		std::vector<std::string> words;

		words = parseLine(input);

		// Redirection Handling
		bool redirectStdout = false;
		bool redirectStderr = false;
		bool appendStdout = false;
		bool appendStderr = false;

		// Filename for redirection
		std::string filename;

		// Check for Empty
		if (words.empty())
		{ 
			continue;
		}

		for (size_t i = 0; i < words.size(); i++)
		{
			// Redirecting stdout to a file
			if (words[i] == ">" || words[i] == "1>")
			{
				redirectStdout = true;

				if (i + 1 < words.size())
				{
					filename = words[i + 1];
				}

				// Entferne > und filename aus Argumentliste
				words.erase(words.begin() + i, words.begin() + i + 2);
				break;
			}

			// Redirecting stderr to a file
			if (words[i] == "2>")
			{
				redirectStderr = true;

				if (i + 1 < words.size())
				{
					filename = words[i + 1];
				}

				// Entferne 2> und filename aus Argumentliste
				words.erase(words.begin() + i, words.begin() + i + 2);
				break;
			}

			// Append stdout to a file
			if (words[i] == ">>" || words[i] == "1>>")
			{
				appendStdout = true;

				if (i + 1 < words.size())
				{
					filename = words[i + 1];
				}

				// Entferne >> und filename aus Argumentliste
				words.erase(words.begin() + i, words.begin() + i + 2);
				break;
			}

			// Append stderr to a file
			if (words[i] == "2>>")
			{
				appendStderr = true;

				if (i + 1 < words.size())
				{
					filename = words[i + 1];
				}

				// Entferne 2>> und filename aus Argumentliste
				words.erase(words.begin() + i, words.begin() + i + 2);
				break;
			}
		}

		// Leave the Shell
		if (words[0] == "exit")
		{
			break;
		}

#pragma region echo
		// Echo Command
		else if (words[0] == "echo")
		{
			if (redirectStdout)
			{
				// Ensure parent directory exists
				fs::path filePath(filename);
				if (!fs::exists(filePath.parent_path()))
				{
					fs::create_directories(filePath.parent_path());
				}

				std::ofstream outfile(filename);
				if (!outfile)
				{
					std::cerr << "Fehler beim Öffnen der Datei\n";
					continue;
				}

				for (size_t i = 1; i < words.size(); i++)
				{
					outfile << words[i];
					if (i + 1 < words.size()) outfile << " ";
				}
				outfile << std::endl;
			}

			// Append Stdout
			else if (appendStdout)
			{
				// Ensure parent directory exists
				fs::path filePath(filename);
				if (!fs::exists(filePath.parent_path()))
				{
					fs::create_directories(filePath.parent_path());
				}

				std::ofstream outfile(filename, std::ios::app); // <-- append mode
				if (!outfile)
				{
					std::cerr << "Fehler beim Öffnen der Datei\n";
					continue;
				}

				for (size_t i = 1; i < words.size(); i++)
				{
					outfile << words[i];
					if (i + 1 < words.size()) outfile << " ";
				}
				outfile << std::endl;
			}

			// NOT redirecting, just echo to stdout
			else
			{
				// Inside your echo redirection code
				if (redirectStderr)
				{
					fs::path filePath(filename);

					// Create parent directories if they don't exist
					if (!filePath.parent_path().empty() && !fs::exists(filePath.parent_path()))
					{
						fs::create_directories(filePath.parent_path());
					}

					std::ofstream outfile(filename);
					if (!outfile)
					{
						std::cerr << "Fehler beim Öffnen der Datei\n";
						continue;
					}
				} 

				// Append to the file if appending
				else if (appendStderr)
				{
					fs::path filePath(filename);

					// Create parent directories if they don't exist
					if (!filePath.parent_path().empty() && !fs::exists(filePath.parent_path()))
					{
						fs::create_directories(filePath.parent_path());
					}

					std::ofstream outfile(filename, std::ios::app); // <-- append mode
					if (!outfile)
					{
						std::cerr << "Fehler beim Öffnen der Datei\n";
						continue;
					}
				}

				// Write normal Output to the console
				for (size_t i = 1; i < words.size(); i++)
				{
					std::cout << words[i];
					if (i + 1 < words.size()) std::cout << " ";
				}
				std::cout << std::endl;
			}
		}
#pragma endregion

		// View the current Directory
		else if (words[0] == "pwd")
		{
			std::cout << fs::current_path().string() << "\n";
		}

		// Change Directory
		else if (words[0] == "cd")
		{
			if (words.size() < 2)
			{
				continue; // nichts tun
			}

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
			runProcess(words,redirectStdout, redirectStderr,
				appendStdout, appendStderr, filename);
		}

		// Default
		else
		{
			std::cout << input << ": command not found\n";
		}
	}

	return 0;
}
