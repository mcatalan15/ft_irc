#include "../include/Server.hpp"

void	Server::passCmd(string cmd, int fd){
	std::cout << "PASS cmd" << std::endl;
}

void	Server::nickCmd(string cmd, int fd){
	std::cout << "NICK cmd" << std::endl;
}

void	Server::userCmd(string cmd, int fd){
	std::cout << "USER cmd" << std::endl;
}

void	Server::quitCmd(string cmd, int fd){
	std::cout << "QUIT cmd" << std::endl;
}

void	Server::modeCmd(string cmd, int fd){
	std::cout << "MODE cmd" << std::endl;
}

void	Server::joinCmd(string cmd, int fd){
	std::cout << "JOIN cmd" << std::endl;
}

void	Server::partCmd(string cmd, int fd){
	std::cout << "PART cmd" << std::endl;
}

void	Server::topicCmd(string cmd, int fd){
	std::cout << "TOPIC cmd" << std::endl;
}

void	Server::kickCmd(string cmd, int fd){
	std::cout << "KICK cmd" << std::endl;
}

void	Server::privmsgCmd(string cmd, int fd){
	std::cout << "PRIVMSG cmd" << std::endl;
}

void	Server::inviteCmd(string cmd, int fd){
	std::cout << "INVITE cmd" << std::endl;
}

void	Server::whoisCmd(string cmd, int fd){
	std::cout << "WHOIS cmd" << std::endl;
}

void	Server::adminCmd(string cmd, int fd){
	std::cout << "ADMIN cmd" << std::endl;
}

void	Server::infoCmd(string cmd, int fd){
	std::cout << "INFO cmd" << std::endl;
}

void	Server::pongCmd(string cmd, int fd){
	std::cout << "PONG cmd" << std::endl;
}

void	Server::pingCmd(string cmd, int fd){
	std::cout << "PING cmd" << std::endl;
}
