#include "../include/Server.hpp"
#include <iostream>

void	Server::passCmd(string &cmd, int fd){
	if (getClient(fd)->getState()!= HANDSHAKE)
		return ;
	else {
		if (cmd == getPassword()) {
			getClient(fd)->setState(NICK);
			std::cout << "Correct password!" << std::endl;
		}
		//PASSword incorrect message
	}
}

bool	Server::nickIsUsed(string cmd) {
	for (size_t i = 0; i < _clients.size(); i++) {
		std::cout << "cmd: " << cmd << "  client: " << _clients[i].getNickname() << std::endl;
		if (_clients[i].getNickname() == cmd) {
			std::cout << "Exists" << std::endl;
			return false;
		}
	}
	return true;
}

void	Server::nickCmd(string &cmd, int fd){
	std::cout << "NICK cmd" << std::endl;
	if (getClient(fd)->getState() == NICK || getClient(fd)->getState() == REGISTERED) {
		if (!nickIsUsed(cmd))
			sendMsg("Nickname used\n", fd); // cambiar al error q toca
		else {
				getClient(fd)->setState(LOGIN);
				getClient(fd)->setNickname(cmd);
				std::cout << "Nickname: " << getClient(fd)->getNickname() << std::endl;
		}
	}
	return ;
}

bool	Server::userIsUsed(string cmd) {
	
	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << "cmd: <" << cmd << ">  user: <" << _clients[i].getUsername() << ">" << std::endl;
		if (_clients[i].getUsername() == cmd) {
			std::cout << "Exists" << std::endl;
			return false;
		}
	}
	return true;
}

void	Server::userCmd(string &cmd, int fd){
	std::cout << "USER cmd" << std::endl;
	if (getClient(fd)->getState() == LOGIN) {
		std::cout << "entra if general" << std::endl;
		if (!userIsUsed(cmd))
			sendMsg("Username used\n", fd);
		else {
			std::vector<string> uSplited = splitUserCmd(cmd);
			for (size_t i = 0; i < uSplited.size(); i++)
				std::cout << "cmd[" << i << "] <" << uSplited[i] << ">" << std::endl;
			getClient(fd)->setUsername(cmd);
			getClient(fd)->setState(REGISTERED);
			std::cout << "Username: " << getClient(fd)->getUsername() << std::endl;
			std::cout << GREEN << "CONNECTED AND REGISTERED!!!!!" << RESET << std::endl;
		}
	}
	return ;
}

void	Server::quitCmd(string &cmd, int fd){
	std::cout << "QUIT cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::modeCmd(string &cmd, int fd){
	std::cout << "MODE cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::joinCmd(string &cmd, int fd){
	std::cout << "JOIN cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::partCmd(string &cmd, int fd){
	std::cout << "PART cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::topicCmd(string &cmd, int fd){
	std::cout << "TOPIC cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::kickCmd(string &cmd, int fd){
	std::cout << "KICK cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::privmsgCmd(string &cmd, int fd){
	std::cout << "PRIVMSG cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::inviteCmd(string &cmd, int fd){
	std::cout << "INVITE cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::whoisCmd(string &cmd, int fd){
	std::cout << "WHOIS cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::adminCmd(string &cmd, int fd){
	std::cout << "ADMIN cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::infoCmd(string &cmd, int fd){
	std::cout << "INFO cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::pongCmd(string &cmd, int fd){
	std::cout << "PONG cmd" << std::endl;
	(void)cmd;
	(void)fd;
}

void	Server::pingCmd(string &cmd, int fd){
	std::cout << "PING cmd" << std::endl;
	(void)cmd;
	(void)fd;
}
