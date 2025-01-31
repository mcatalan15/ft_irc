#include "../include/Irc.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
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
			if (!currentWord.empty())
				result.push_back(currentWord);
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

// Split buffer when there is some commands in the same buffer
std::vector<string>	splitCommand(string &cmd) {
	std::vector<string>	splited;
	std::istringstream	stm(cmd);
	string				tmp;
	
	while (std::getline(stm, tmp, '\n')) {
		if (tmp.find_last_of("\r\n"))
			tmp.erase(tmp.size() - 1, tmp.size());
		splited.push_back(tmp);
	}
	return splited;
}

// Split the command in the vector with the correct division
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

// Print the any vector of strings
void	printVecStr(std::vector<string> cmd) {
	for (size_t i = 0; i < cmd.size(); i++)
		std::cout << "cmd[" << i << "] <" << cmd[i] << ">" << std::endl;
}

// Use gethostname to get the hostname of the machine
string	addHostname() {
	char	hostname[256];
	
	if (gethostname(hostname, sizeof(hostname)) == 0)
		return string(hostname);
	return string(); //Debiria el hostname ser localhost????
}

void	sendMsg(string msg, int fd) {
	send(fd, msg.c_str(), msg.size(), 0);
}
