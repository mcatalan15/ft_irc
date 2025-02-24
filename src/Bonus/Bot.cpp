#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <algorithm>

using std::string;

class IRCBot {
	public:
		IRCBot(const string& host, const std::string& port, const std::string& password, const std::string& channel);
		~IRCBot();

		bool connectToServer();
		void sendCommand(const string& command);
		void login();
		void run();

	private:
		string host_;
		string port_;
		string password_;
		string channel_;
		int sockfd_;
		std::map<string, string> commands_;
		string toLower(const string& str);
		string trim(const string& str);
		string extractMessageContent(const string& message);
		string removeCRLF(const string& str);
};

// Constructor
IRCBot::IRCBot(const string& host, const string& port, const string& password, const string& channel)
	: host_(host), port_(port), password_(password), channel_(channel), sockfd_(-1) {
		// Initialize commands
		commands_["quien fuma?"] = "el puma";
		commands_["8"] = "bingo";
		commands_["7"] = "linea!!!!";
		commands_["klk"] = "manin";
		commands_["pain au chocolat o chocolatine?"] = "napolitana";
		commands_["coca y keta?"] = "VOLTERETA";
		commands_["BOOOMBA"] = "sensual";
		commands_["quien lo peta?"] = "el mas fumeta";
		commands_["amor porreta?"] = "pal que lo peta";
		commands_["Maestro yoda, que es la fuerza?"] = "Es la p...a que cada manana te almuerzas";
}

// Destructor
IRCBot::~IRCBot() {
	if (sockfd_ != -1)
		close(sockfd_);
}

// Convert string to lowercase
string IRCBot::toLower(const string& str) {
	string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

// Trim leading and trailing whitespace
string IRCBot::trim(const std::string& str) {
	size_t first = str.find_first_not_of(' ');
	if (first == string::npos) return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}

// Remove \r\n from the end of the string (C++98 compatible)
string IRCBot::removeCRLF(const std::string& str) {
	string cleanedStr = str;
	if (!cleanedStr.empty() && cleanedStr[cleanedStr.size() - 1] == '\n') {
		cleanedStr.erase(cleanedStr.size() - 1);
	}
	if (!cleanedStr.empty() && cleanedStr[cleanedStr.size() - 1] == '\r') {
		cleanedStr.erase(cleanedStr.size() - 1);
	}
	return cleanedStr;
}

// Extract the message content (after the last ':')
string IRCBot::extractMessageContent(const std::string& message) {
	size_t colonPos = message.find_last_of(':');
	if (colonPos == string::npos) return "";
	return trim(message.substr(colonPos + 1));
}

// Connect to the IRC server
bool IRCBot::connectToServer() {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(host_.c_str(), port_.c_str(), &hints, &res) != 0) {
		std::cerr << "Error: Could not resolve hostname." << std::endl;
		return false;
	}

	sockfd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd_ == -1) {
		std::cerr << "Error: Could not create socket." << std::endl;
		freeaddrinfo(res);
		return false;
	}

	if (connect(sockfd_, res->ai_addr, res->ai_addrlen) == -1) {
		std::cerr << "Error: Could not connect to server." << std::endl;
		freeaddrinfo(res);
		close(sockfd_);
		sockfd_ = -1;
		return false;
	}

	freeaddrinfo(res);
	return true;
}

// Send a command to the IRC server
void IRCBot::sendCommand(const string& command) {
	string fullCommand = command + "\r\n";
	if (send(sockfd_, fullCommand.c_str(), fullCommand.length(), 0) == -1)
		std::cerr << "Error: Failed to send command." << std::endl;
}

// Log in to the IRC server and join the channel
void IRCBot::login() {
	sendCommand("PASS " + password_);
	sendCommand("NICK IRCBot");
	sendCommand("USER IRCBot 0 * :IRC Bot");
	sendCommand("JOIN #" + channel_); // Join the channel directly
}

// Main loop to handle incoming messages
void IRCBot::run() {
	char buffer[512];
	while (true) {
		memset(buffer, 0, sizeof(buffer));
		int bytesReceived = recv(sockfd_, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived <= 0) {
			std::cerr << "Error: Connection lost." << std::endl;
			break;
		}

		string message(buffer, bytesReceived);
		message = removeCRLF(message);
		std::cout << "Received: " << message << std::endl;

		if (message.find("PING") == 0)
			sendCommand("PONG" + message.substr(5));

		size_t privmsgPos = message.find("PRIVMSG");
		if (privmsgPos != string::npos) {
			size_t channelStart = message.find("#", privmsgPos);
			size_t channelEnd = message.find(" ", channelStart);
			string targetChannel = message.substr(channelStart, channelEnd - channelStart);

			if (targetChannel == "#" + channel_) {
				string userMessage = extractMessageContent(message);
				userMessage = toLower(userMessage); // Convert to lowercase

				std::map<string, std::string>::iterator it = commands_.find(userMessage);
				if (it != commands_.end())
					sendCommand("PRIVMSG #" + channel_ + " :" + it->second);
			}
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 5) {
		std::cerr << "Usage: " << argv[0] << " <host> <port> <password> <channel>" << std::endl;
		return 1;
	}

	IRCBot bot(argv[1], argv[2], argv[3], argv[4]);

	if (!bot.connectToServer())
		return 1;

	bot.login();
	bot.run();

	return 0;
}
