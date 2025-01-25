#include "../include/Irc.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

std::vector<string> splitMsg(string &str) {
    std::vector<std::string> result;
    size_t firstSpace = str.find_first_of(" \t");
    
    // Extract the first word
    result.push_back(str.substr(0, firstSpace));
    
    // Extract the remainder, trimming leading spaces
    if (firstSpace != std::string::npos) {
        std::string remainder = str.substr(firstSpace + 1);
        remainder.erase(0, remainder.find_first_not_of(" \t"));
        result.push_back(remainder);
    } else {
        result.push_back(""); // Add an empty string if there's no remainder
    }
    
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
