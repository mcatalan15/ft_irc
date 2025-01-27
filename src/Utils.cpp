#include "../include/Irc.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// CHECK NEW SPLITMSG. CREATES EXTRA VECTOR IN : CASE
std::vector<std::string> splitMsg(std::string &str) {
	std::vector<std::string> result;
	std::string currentWord;
	bool foundColon = false;

	for (std::string::size_type i = 0; i < str.size(); ++i) {
		char c = str[i];
		if (foundColon) {
			// After finding ':', add the rest of the string as a single command
			currentWord += c;
		} else if (c == ':') {
			// Start treating everything after ':' as a single command
			foundColon = true;
			result.push_back(currentWord);
			//currentWord = c; // Include ':' in the current word
		} else if (c == ' ' || c == '\t') {
			// Split words on spaces or tabs
			if (!currentWord.empty()) {
				result.push_back(currentWord);
				currentWord.clear();
			}
		} else {
			// Accumulate characters into the current word
			currentWord += c;
		}
	}
	// Add the last word if it's non-empty
	if (!currentWord.empty())
        result.push_back(currentWord);
    return result;
}

string	getCommandInUpper(const string &cmd) {
	size_t start = cmd.find_first_not_of(" \t\v"); // Avoid initial empty spaces
	if (start == string::npos)
		return "";
		
	size_t end = cmd.find_first_of(" \t\v", start); // Find the end of the first word
	std::string command = cmd.substr(start, end - start);

	// Check if the command is already uppercase
	bool isUpper = true;
	for (size_t i = 0; i < command.size(); ++i) {
		if (!std::isupper(command[i])) {
			isUpper = false;
			break;
		}
	}
	if (isUpper)
		return command;

	// Convert the command to uppercase
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);
	return command;
}

std::vector<string>	splitCommand(string &cmd) {
	std::vector<string>	splited;
	std::istringstream	stm(cmd);
	string				tmp;
	
	while (stm >> tmp) {
		splited.push_back(tmp);
		tmp.clear();
	}
	return splited;
}

std::vector<string> splitUserCmd(string& str)
{
	size_t				i;
	size_t				init = 0;
	std::vector<string>	vec;
	
	for (i = 0; str[i] != ':' && i < str.size(); i++)
	{	
		if (str[i] != ' ') // && str[i + 1] == ' ')
		{
			std::cout << "entra" << std::endl;
			init = i;
			while (str[i] != ' ' && str[i])
				i++;
			vec.push_back(str.substr(init, i - init));
		}
	}
	vec.push_back(str.substr(i, -1));
	return vec;
}
