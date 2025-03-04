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
		string _host;
		string _port;
		string _passwd;
		string _channel;
		int _sockfd;
		std::map<string, string> _cmds;
		string toLower(const string& str);
		string trim(const string& str);
		string extractMessageContent(const string& message);
		string removeCRLF(const string& str);
};

// Constructor
IRCBot::IRCBot(const string& host, const string& port, const string& password, const string& channel)
	: _host(host), _port(port), _passwd(password), _channel(channel), _sockfd(-1) {
		_cmds["quien fuma?"] = "el puma";
		_cmds["8"] = "bingo";
		_cmds["7"] = "linea!!!!";
		_cmds["klk"] = "manin";
		_cmds["pain au chocolat ou chocolatine?"] = "napolitana";
		_cmds["coca y keta?"] = "VOLTERETA";
		_cmds["BOOOMBA"] = "sensual";
		_cmds["quien lo peta?"] = "el mas fumeta";
		_cmds["amor porreta?"] = "pal que lo peta";
		_cmds["quien lo mata?"] = "el mas rata";
}

IRCBot::~IRCBot() {
	if (_sockfd != -1)
		close(_sockfd);
}

string IRCBot::toLower(const string& str) {
	string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

string IRCBot::trim(const std::string& str) {
	size_t first = str.find_first_not_of(' ');
	if (first == string::npos) return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}

string IRCBot::removeCRLF(const std::string& str) {
	string cleanedStr = str;
	if (!cleanedStr.empty() && cleanedStr[cleanedStr.size() - 1] == '\n')
		cleanedStr.erase(cleanedStr.size() - 1);
	if (!cleanedStr.empty() && cleanedStr[cleanedStr.size() - 1] == '\r')
		cleanedStr.erase(cleanedStr.size() - 1);
	return cleanedStr;
}

string IRCBot::extractMessageContent(const std::string& message) {
	size_t colonPos = message.find_last_of(':');
	if (colonPos == string::npos)
		return "";
	return trim(message.substr(colonPos + 1));
}

bool IRCBot::connectToServer() {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(_host.c_str(), _port.c_str(), &hints, &res) != 0) {
		std::cerr << "Error: Could not resolve hostname." << std::endl;
		return false;
	}

	_sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_sockfd == -1) {
		std::cerr << "Error: Could not create socket." << std::endl;
		freeaddrinfo(res);
		return false;
	}

	if (connect(_sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		std::cerr << "Error: Could not connect to server." << std::endl;
		freeaddrinfo(res);
		close(_sockfd);
		_sockfd = -1;
		return false;
	}

	freeaddrinfo(res);
	return true;
}

void IRCBot::sendCommand(const string& command) {
	string fullCommand = command + "\r\n";
	if (send(_sockfd, fullCommand.c_str(), fullCommand.length(), 0) == -1)
		std::cerr << "Error: Failed to send command." << std::endl;
}

void IRCBot::login() {
	sendCommand("PASS " + _passwd);
	sendCommand("NICK IRCBot");
	sendCommand("USER IRCBot 0 * :IRC Bot");
	sendCommand("JOIN #" + _channel);
}

void IRCBot::run() {
	char buffer[512];
	while (true) {
		memset(buffer, 0, sizeof(buffer));
		int bytesReceived = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
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

			if (targetChannel == "#" + _channel) {
				string userMessage = extractMessageContent(message);
				userMessage = toLower(userMessage);

				std::map<string, std::string>::iterator it = _cmds.find(userMessage);
				if (it != _cmds.end())
					sendCommand("PRIVMSG #" + _channel + " :" + it->second);
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
