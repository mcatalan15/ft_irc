#include "../include/Server.hpp"
#include <iostream>
#include <string>
#include <vector>

// CAP COMMAND
void	Server::capCmd(std::vector<std::string>& cmd, int fd) {
	std::cout << "CAP cmd" << std::endl;
	if (cmd[1] == "LS")
		sendMsg("CAP * LS :0\r\n", fd);
	//if (cmd[1] == "END")
		//sendMsg("CAP END\r\n", fd);	
	std::cout << "message send" << std::endl;
	(void)cmd;
}

// PASSWORD COMMAND
void	Server::passCmd(std::vector<string>& cmd, int fd){
	if (cmd.size() < 2 || cmd[1].empty()) {
		sendMsg("Password not provided", fd);
		return ;
	}
	// Soluciona el error??
	Client *client = getClient(fd);
	if (!client) {
		sendMsg("Server Error: Client not found", fd);
		return ;
	}
	if (getClient(fd)->getState()!= HANDSHAKE)
		return ;
	else {
		if (cmd[1] == getPassword()) {
			getClient(fd)->setState(NICK);
			std::cout << "Correct password!" << std::endl;
			sendMsg("Correct password\r\n", fd);
		}
		else
			sendMsg("password incorrect msg", fd);
	}
}

// NICKNAME COMMAND
bool	Server::nickIsUsed(string cmd) {
	for (size_t i = 0; i < _clients.size(); i++) {
		std::cout << "cmdssss: " << cmd << "  client: " << _clients[i].getNickname() << std::endl;
		if (_clients[i].getNickname() == cmd) {
			std::cout << "Exists" << std::endl;
			return false;
		}
	}
	return true;
}

void	Server::nickCmd(std::vector<string>& cmd, int fd){
	std::cout << "NICK cmd" << std::endl;
	if (getClient(fd)->getState() == NICK || getClient(fd)->getState() == REGISTERED) {
		if (cmd[1].size() > 9)
			sendMsg("Nickname too long (>9)\n", fd); // cambiar al error q toca
		else if (!nickIsUsed(cmd[1]))
			sendMsg("Nickname used\n", fd); // cambiar al error q toca
		else {
				getClient(fd)->setState(LOGIN);
				getClient(fd)->setNickname(cmd[1]);
				std::cout << "Nickname: " << getClient(fd)->getNickname() << std::endl;
				sendMsg("correct nickname\r\n", fd);
		}
	}
	return ;
}

// USER COMMAND
bool	Server::userIsUsed(string cmd) {
	// REALMENTE CUAL DEBERIA DE TODOS LOS NOMBRES DEBE MIRAR???
	for (size_t i = 0; i < _clients.size(); i++) {
		std::cout << "cmd: <" << cmd << ">  user: <" << _clients[i].getUsername() << ">" << std::endl;
		if (_clients[i].getUsername() == cmd) {
			std::cout << "Exists" << std::endl;
			return false;
		}
	}
	return true;
}

void	Server::userCmd(std::vector<string>& cmd, int fd){
	std::cout << "USER cmd" << std::endl;
	printVecStr(cmd);
	std::cout << "User len <" << cmd.size() << ">" << std::endl;
	if (getClient(fd)->getState() == LOGIN && cmd.size() == 5) {
		std::cout << "entra if general" << std::endl;
		if (!userIsUsed(cmd[1]))
			sendMsg("Username used\n", fd);
		else {
			getClient(fd)->setUsername(cmd[1]);
			getClient(fd)->setRealname(cmd[3]);
			getClient(fd)->setState(REGISTERED);
			std::cout << "Username: " << getClient(fd)->getUsername() << std::endl;
			std::cout << GREEN << "CONNECTED AND REGISTERED!!!!!" << RESET << std::endl;
			getClient(fd)->welcome(*getClient(fd), fd);
		}
	}
	return ;
}

// QUIT COMMAND
void	Server::quitCmd(std::vector<string>& cmd, int fd){  			// falta hacer que envie cmd a todos los usuarios que cmpartan canales.
	std::cout << "QUIT cmd" << std::endl;
	string empty;
	empty.clear();
	getClient(fd)->setMsg(empty);
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->getFd() == fd)
		{
			_clients.erase(it);
			break;
		}
	}
	for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); it++) {
		if (it->fd == fd)
		{
			_pollFds.erase(it);
			break;
		}
	}
	close(fd);
	(void)cmd;
}

// MODE COMMAND
void	Server::modeCmd(std::vector<string>& cmd, int fd){
	std::cout << "MODE cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// JOIN COMMAND
void	Server::joinCmd(std::vector<string>& cmd, int fd){
	std::cout << "JOIN cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// PART COMMAND
void	Server::partCmd(std::vector<string>& cmd, int fd){
	std::cout << "PART cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

//TOPIC COMMAND
void	Server::topicCmd(std::vector<string>& cmd, int fd){
	std::cout << "TOPIC cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// KICK COMMAND
void	Server::kickCmd(std::vector<string>& cmd, int fd){
	std::cout << "KICK cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// PRIVMSG COMMAND
void	Server::privmsgCmd(std::vector<string>& cmd, int fd){
	std::cout << "PRIVMSG cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// INVITE COMMAND
void	Server::inviteCmd(std::vector<string>& cmd, int fd){
	std::cout << "INVITE cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// WHOIS COMMAND
void	Server::whoisCmd(std::vector<string>& cmd, int fd){
	std::cout << "WHOIS cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// ADMIN COMMAND
void	Server::adminCmd(std::vector<string>& cmd, int fd){
	std::cout << "ADMIN cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// INFO COMMAND
void	Server::infoCmd(std::vector<string>& cmd, int fd){
	std::cout << "INFO cmd" << std::endl;
	(void)cmd;
	(void)fd;
	
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "╔════════════════════════════════════════╗"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║      Welcome to ExampleIRC Server      ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║         Powered by InspIRCd 3.0         ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "╠════════════════════════════════════════╣"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║ Admin: AdminNick  (admin@example.com)  ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║ Uptime: 12 days, 4 hours, 32 minutes    ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║ Users Online: 128                       ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║ Channels: 45                            ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "╠════════════════════════════════════════╣"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  Rules:                                 ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  1. Be respectful.                      ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  2. No spam or flooding.                ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  3. No excessive trolling.               ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  4. Follow channel-specific rules.      ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "╠════════════════════════════════════════╣"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  Need help? /join #help                 ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  Visit: https://www.example.com         ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "╚════════════════════════════════════════╝"), fd);
	sendMsg(RPL_ENDOFINFO(getClient(fd)->getNickname()), fd);
}

// PONG COMMAND
void	Server::pongCmd(std::vector<string>& cmd, int fd){
	std::cout << "PONG cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

// PING COMMAND
void	Server::pingCmd(std::vector<string>& cmd, int fd){
	std::cout << "PING cmd" << std::endl;
	string tmp = cmd[1].append(CRLF);
	string pong = "PONG ";
	sendMsg(pong.append(tmp), fd);
}
