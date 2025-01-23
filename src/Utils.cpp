#include "../include/Irc.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <sstream>
#include <string>

std::vector<string>	splitMsg(string str) {
	std::vector<string>	command;
	std::istringstream	stm(str);
	string				line;
	
	while(std::getline(stm, line)) {
		size_t pos = line.find_first_of("\r\n");
		if (pos != string::npos)
			line = line.substr(0, pos);
		command.push_back(line);
	}
	return command;
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
