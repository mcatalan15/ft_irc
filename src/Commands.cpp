#include "../include/Server.hpp"
#include <iostream>
#include <string>
#include <vector>

void	Server::capCmd(std::vector<std::string>& cmd, int fd) {
	std::cout << "CAP cmd" << std::endl;
	if (cmd[1] == "LS")
		sendMsg("CAP * LS :0\r\n", fd);
	if (cmd[1] == "END")
		sendMsg("CAP END\r\n", fd);	
	std::cout << "message send" << std::endl;
	(void)cmd;
}

void	Server::passCmd(std::vector<string>& cmd, int fd){
	if (getClient(fd)->getState()!= HANDSHAKE)
		return ;
	else {
		if (cmd[1] == getPassword()) {
			getClient(fd)->setState(NICK);
			std::cout << "Correct password!" << std::endl;
			sendMsg("Correct password\r\n", fd);
		}
		//PASSword incorrect message
//		sendMsg("password incorrect msg", fd);
	}
}

bool	Server::nickIsUsed(string cmd) {
	for (size_t i = 0; i < _clients.size(); i++) {
		std::cout << "cmd: " << cmd[1] << "  client: " << _clients[i].getNickname() << std::endl;
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
		if (!nickIsUsed(cmd[1]))
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
		//std::vector<string> uSplited = splitUserCmd(cmd);
		//for (size_t i = 0; i < uSplited.size(); i++)
		//	std::cout << "cmd[" << i << "] <" << uSplited[i] << ">" << std::endl;
		if (!userIsUsed(cmd[1]))
			sendMsg("Username used\n", fd);
		else {
			getClient(fd)->setUsername(cmd[1]);
			getClient(fd)->setState(REGISTERED);
			getClient(fd)->setRealname(cmd[3]);
			std::cout << "Username: " << getClient(fd)->getUsername() << std::endl;
			std::cout << GREEN << "CONNECTED AND REGISTERED!!!!!" << RESET << std::endl;
			sendMsg("Correct used\r\n", fd);
		}
	}
	return ;
}

void	Server::quitCmd(std::vector<string>& cmd, int fd){
	std::cout << "QUIT cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::modeCmd(std::vector<string>& cmd, int fd){
	std::cout << "MODE cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::joinCmd(std::vector<string>& cmd, int fd){
	std::cout << "JOIN cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::partCmd(std::vector<string>& cmd, int fd){
	std::cout << "PART cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::topicCmd(std::vector<string>& cmd, int fd){
	std::cout << "TOPIC cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::kickCmd(std::vector<string>& cmd, int fd){
	std::cout << "KICK cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::privmsgCmd(std::vector<string>& cmd, int fd){
	std::cout << "PRIVMSG cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::inviteCmd(std::vector<string>& cmd, int fd){
	std::cout << "INVITE cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::whoisCmd(std::vector<string>& cmd, int fd){
	std::cout << "WHOIS cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::adminCmd(std::vector<string>& cmd, int fd){
	std::cout << "ADMIN cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::infoCmd(std::vector<string>& cmd, int fd){
	std::cout << "INFO cmd" << std::endl;
	(void)cmd;
	sendMsg("RPL_INFO(371)", fd);
	sendMsg("RPL_ENDOFINFO(374)\r\n", fd);
}

void	Server::pongCmd(std::vector<string>& cmd, int fd){
	std::cout << "PONG cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::pingCmd(std::vector<string>& cmd, int fd){
	std::cout << "PING cmd" << std::endl;
	(void)cmd;
	(void)fd;
}
