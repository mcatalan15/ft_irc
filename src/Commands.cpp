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
	Client *client = getClient(fd);

	if (cmd.size() < 2 || cmd[1].empty())
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), cmd[0]), fd));
	if (!client)
		return (sendMsg("Server Error: Client not found", fd));
	if (client->getState() != HANDSHAKE)
	{
		if (client->getState() == REGISTERED)
			sendMsg(ERR_ALREADYREGISTERED(client->getNickname()), fd);
		return ;
	}
	else {
		if (cmd[1] == getPassword()) {
			getClient(fd)->setState(NICK);
			std::cout << "Correct password!" << std::endl;
			sendMsg("Correct password\r\n", fd);
		}
		else
			sendMsg(ERR_PASSWDMISMATCH(client->getNickname()), fd);
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
	Client *client = getClient(fd);
	if (cmd.size() < 2 || cmd[1].empty())
		return (sendMsg(ERR_NONICKNAMEGIVEN(), fd));
	if (client->getState() == NICK || client->getState() == REGISTERED) {
		if (!nickChecker(cmd[1]))
			sendMsg(ERR_ERRONEUSNICKNAME(cmd[1]), fd); // cambiar al error q toca
		else if (!nickIsUsed(cmd[1]))
			sendMsg(ERR_NICKNAMEINUSE(cmd[1]), fd); // cambiar al error q toca
		else {
				client->setState(LOGIN);
				client->setNickname(cmd[1]);
				std::cout << "Nickname: " << client->getNickname() << std::endl;
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
	Client*	client = getClient(fd);

	std::cout << "USER cmd" << std::endl;
	printVecStr(cmd);
	std::cout << "User len <" << cmd.size() << ">" << std::endl;
	if (cmd.size() < 2 || cmd[1].empty())
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), cmd[1]), fd));
	if (client->getState() == LOGIN && cmd.size() == 5) {
		std::cout << "entra if general" << std::endl;
		if (cmd[1].size() > 18)
			cmd[1] = cmd[1].substr(0, 18);
		if (!userIsUsed(cmd[1]))
			sendMsg("Username is used\r\n", fd);
			//sendMsg(ERR_USERNAMEINUSE(client->getUsername()), fd);??????????????
		else {
			client->setUsername(cmd[1]);
			client->setRealname(cmd[3]);
			client->setState(REGISTERED);
			std::cout << "Username: " << client->getUsername() << std::endl;
			std::cout << GREEN << "CONNECTED AND REGISTERED!!!!!" << RESET << std::endl;
			client->welcome(*getClient(fd), fd);
		}
	}
	else
		sendMsg(ERR_ALREADYREGISTERED(client->getNickname()), fd);
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

// ------------------------------------------------------------------------

/*void	Server::successJoin(int fd) {
	//get channel
	sendMsg();
	}*/
// JOIN COMMAND

std::vector<string>	joinDivisor(string cmd) {

	size_t	i;
	size_t	init = 0;
	std::vector<string>	vec;
	int		num_ch = 0;
	for (i = 0; i < cmd.size(); i++) {
		if (cmd[i] == ',')
		{
			vec.push_back(cmd.substr(init, i - init));
			//std::cout << "cmd[" << num_ch << "]: " << cmd[i] << std::endl;
			init = i + 1;
			num_ch++;
		}
	}
	vec.push_back(cmd.substr(init, -1));
	//std::cout << "cmd[" << num_ch << "]: " <<  << std::endl;
	return vec;
}
void	Server::joinCmd(std::vector<string>& cmd, int fd)
{
	std::cout << "JOIN cmd" << std::endl;
	Client*		client = getClient(fd);
	// Checkers
	if (cmd.size() < 2)
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), cmd[0]), fd));
	if (cmd[1][0] != '#') // CAMBIAR!!!! SOO aplica al 1r
		return (sendMsg("channel needs to start with '#'\n", fd));
	
	std::cout << "CANALES!!!" << std::endl;
	std::vector<string> channelsNames = joinDivisor(cmd[1]);
	printVecStr(channelsNames);
	std::vector<string> channelsPass;
	if (cmd.size() > 2) {
		std::cout << std::endl << "KEYYSSS!!!!" <<std::endl;
		channelsPass = joinDivisor(cmd[2]);
		printVecStr(channelsPass);
	}

	// Check if channel exist
	for (size_t i = 0; i < channelsNames.size(); i++)
	{
		Channel*	found = NULL;
		std::cout << "empieza" << _channels.size() << "Channel: " << channelsNames.size() << std::endl;
		for (size_t j = 0; j < _channels.size(); j++)
		{
			if (_channels[j].getName() == channelsNames[i]) {
				found = &_channels[j];
				break ;
			}
		}
		// If channel doesnt exist
		if (!found)
		{
			//_channels.push_back(channelsNames[i]); //Stores object
			Channel newchannel(channelsNames[i]);
	
			if (cmd.size() > 2 && i < channelsPass.size()) {
				std::cout << "channel: " << channelsNames[i] << "  hay password: " << channelsPass[i] << std::endl;
				newchannel.setPassword(channelsPass[i]);
			}
			else {
				std::cout << "channel: " << channelsNames[i] << "  NO password" << std::endl;
			}
			newchannel.addClient(client);
			newchannel.addOperator(client);
			_channels.push_back(newchannel);
			client->addChannel(&newchannel);
			
			sendMsg("channel " + channelsNames[i] + " created!\n", fd);
		}
		else //Existe el channel
		{
			// mierdas de mode y password
			std::cout << "LA size: " << cmd.size() << std::endl;
			if (cmd.size() > 2) {
				if (i < channelsPass.size() && found->hasPassword()) {
					if (channelsPass[i] == found->getPassword())
						std::cout << "Contrasenya correcte" <<"cdm[" << channelsNames[i] << "] channelpass [" << channelsPass[i]<< "]" << std::endl;
					else
						std::cout << "Contrasenya incorecte" << "cdm[" << channelsNames[i] << "] channelpass [" << channelsPass[i] << "]" << std::endl;
				}
				else {
					std::cout <<" No hay contrasena" << channelsNames[i] << std::endl;
				}
				//std::cout << "Size channelPass" << channelsPass.size() << std::endl;
				//if ((i < channelsPass.size() && found->hasPassword()) || found->getPassword() != channelsPass[i])
				//	std::cout << "Contrasenya incorrecta" << std::endl;
				//found->addClient(client);
				//sendMsg("you joined " + channelsNames[i] + " channel!\n", fd);
			}
		}
		std::cout << "acaba  " << _channels.size() << std::endl;
	}
}
//--------------------------------------------------------------------------------------------
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
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  Visit: https://www.youtube.com/watch?v=dQw4w9WgXcQhttps          ║"), fd);
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
	string tmp = "\0";
	string pong = "PONG ";

	if (cmd.size() > 1)
		tmp = cmd[1].append(CRLF);
	sendMsg(pong.append(tmp), fd);
}
