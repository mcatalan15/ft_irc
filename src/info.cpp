#include "../include/Server.hpp"

string getUpTime(const string &creationTime) {
	struct tm creationTm;
	memset(&creationTm, 2, sizeof(creationTm)); // Properly zero-initialize

	time_t now = time(0);

	// Parse the input string "YYYY-MM-DD HH:MM:SS"
	if (sscanf(creationTime.c_str(), "%d-%d-%d %d:%d:%d",
				&creationTm.tm_year, &creationTm.tm_mon, &creationTm.tm_mday,
				&creationTm.tm_hour, &creationTm.tm_min, &creationTm.tm_sec) != 6) {
		return "Invalid Time Format";
	}

	// Adjust fields to match tm structure
	creationTm.tm_year -= 1900;
	creationTm.tm_mon -= 1;

	// Convert to time_t
	time_t creationTimeT = mktime(&creationTm);
	if (creationTimeT == -1) {
		return "Invalid Time";
	}

	// Calculate the difference in seconds
	time_t uptime = now - creationTimeT;
	if (uptime < 0) {
		return "Future Time Error";
	}

	// Convert uptime into days, hours, minutes, and seconds
	int days = uptime / 86400;
	int hours = (uptime % 86400) / 3600;
	int minutes = (uptime % 3600) / 60;
	int seconds = uptime % 60;

	// Format the uptime string
	std::ostringstream uptimeStr;
	uptimeStr << days << " days, "
		<< (hours < 10 ? "0" : "") << hours << " hours, "
		<< (minutes < 10 ? "0" : "") << minutes << " minuts, "
		<< (seconds < 10 ? "0" : "") << seconds << " seconds";

	return uptimeStr.str();
}

string	horizontalChars(size_t maxLen) {
	string str;
	for (size_t i = 0; i < maxLen; i++)
		str.append(HORIZONTAL);
	return str;
}

string centerText(const string& text, int width) {
	int padding = (width - text.length()) / 2;
	return std::string(padding, ' ') + text + std::string(padding, ' ');
}

string createTableRow(const string& content, int width) {
	return string(VERTICAL) + " " + content + string(width - content.length() - 2, ' ') + " " + string(VERTICAL);
}

std::vector<string> Server::getInfo(void) {
	std::vector<string> infoLines;
	infoLines.push_back("Welcome to FT_IRC Server");
	infoLines.push_back("Powered by eferre-m jpaul-kr mcatalan");
	infoLines.push_back("line");
	infoLines.push_back("Up Time: " + getUpTime(_creationTime));
	infoLines.push_back("Users Online: " + getActiveClients());
	infoLines.push_back("Channels: " + getActiveChannels());
	infoLines.push_back("line");
	infoLines.push_back("Rules:");
	infoLines.push_back("1. Be respectful.");
	infoLines.push_back("2. No spam or flooding.");
	infoLines.push_back("3. No excessive trolling.");
	infoLines.push_back("4. Follow channel-specific rules.");
	infoLines.push_back("line");
	infoLines.push_back("COMMANDS->");
	infoLines.push_back("PASS:");
	infoLines.push_back("  Syntax: PASS <password>");
	infoLines.push_back("NICK:");
	infoLines.push_back("  Syntax: NICK <nickname>");
	infoLines.push_back("USER:");
	infoLines.push_back("  Syntax: USER <username> <hostname> <servername> :<realname>");
	infoLines.push_back("QUIT:");
	infoLines.push_back("  Syntax: QUIT [:<message>]");
	infoLines.push_back("MODE:");
	infoLines.push_back("  Syntax: MODE <target> <mode> [<parameters>]");
	infoLines.push_back("JOIN:");
	infoLines.push_back("  Syntax: JOIN <channel> [<key>]");
	infoLines.push_back("PART:");
	infoLines.push_back("  Syntax: PART <channel> [:<message>]");
	infoLines.push_back("TOPIC:");
	infoLines.push_back("  Syntax: TOPIC <channel> [:<topic>]");
	infoLines.push_back("KICK:");
	infoLines.push_back("  Syntax: KICK <channel> <user> [:<message>]");
	infoLines.push_back("PRIVMSG:");
	infoLines.push_back("  Syntax: PRIVMSG <user|channel> :<message>");
	infoLines.push_back("INVITE:");
	infoLines.push_back("  Syntax: INVITE <user> <channel>");
	infoLines.push_back("INFO:");
	infoLines.push_back("  Syntax: INFO");
	infoLines.push_back("PING:");
	infoLines.push_back("  Syntax: PING <target>");
	infoLines.push_back("DCC:");
	infoLines.push_back("  Syntax: DCC <subcommand> [<parameters>]");
	infoLines.push_back("line");
	infoLines.push_back("Visit: asdggasdgdasgsasgd ");
	return infoLines;
}
