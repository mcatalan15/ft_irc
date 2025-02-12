#include "../include/Server.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// CAP COMMAND
void	Server::capCmd(std::vector<std::string>& cmd, int fd) {
	std::cout << "CAP cmd" << std::endl;
	if (cmd[1] == "LS")
		sendMsg("CAP * LS :0\r\n", fd);
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
			client->welcome(*this, *getClient(fd), fd);
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
		if (it->getFd() == fd) {
			_clients.erase(it);
			break;
		}
	}
	for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); it++) {
		if (it->fd == fd) {
			_pollFds.erase(it);
			break;
		}
	}
	close(fd);
	(void)cmd;
}
// MODE COMMAND ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
bool	Server::isFlagMode(Channel* channel, std::vector<string>& cmd, int num)
{
	(void)channel;
	if (cmd[2].size() < 2) {
		std::cout << "Invalid flag size" << std::endl;      
		return (false);
	}
	if (num == 1)
	{
		for (size_t i = 1; i < cmd[2].size(); i++)
		{
			if (cmd[2].find_first_of(CHANNEL_MODES) == string::npos)
				return (false);
		}
	}
	if (num == 2)
	{
		for (size_t i = 1; i < cmd[2].size(); i++)
		{
			if (cmd[2].find_first_of(CHANNEL_MODES_WITH_PARAM) == string::npos)
				return (false);
		}
	}
	return true;
}


bool	Server::checkModeFlags(Channel* channel, std::vector<string>& cmd, int fd)
{
	if (!isFlagMode(channel, cmd, 2))
	{
		if (!isFlagMode(channel, cmd, 1))
		{
			sendMsg("no such modes...\n", fd);
			return (false);
		}
		return (true);
	}
	// verificar si los comandos son validos
	return (true);
	
}

bool	Server::isModeCmdValid(Channel* channel, std::vector<string>& cmd, int fd)
{
	if (!channel)
	{
		sendMsg(ERR_NOSUCHCHANNEL(getClient(fd)->getNickname(), cmd[1]), fd);
		return (false);
	}
	if (cmd.size() < 3)
	{
		// Y este MSG?
		sendMsg(RPL_CREATIONTIME(getClient(fd)->getNickname(), cmd[1], getCreationTime()), fd);
		return(false);
	}
	if (!channel->isOperator(getClient(fd)))
	{
		sendMsg("buscar que hay que enviar\n", fd);
		return (false);
	}
	// Comprobar que no haya repetidas
	return (true);
}

Client*	Server::findNickname(string nick, Channel* channel)
{
	const std::vector<Client*>&	lstClients = channel->getClients();
	std::cout << "findNickname: " << std::endl;
	(void)nick;
	(void)channel;
	//(void)lstClients;
	//std::cout << "sizeclients: " << channel->getClients().size();
	if (lstClients.size() <= 0) {
		std::cout << "No hay clients en este channel" << std::endl;
		return NULL;
	}
	for (size_t i = 0; i < lstClients.size(); i++)
	{
		if (lstClients[i]->getNickname() == nick)
			return lstClients[i];
	}
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickname() == nick)
			return &_clients[i];
	}
	return NULL;
}

void	Server::modeManagement(Channel* channel, std::vector<string>& cmd, int fd)
{
	(void)fd;
	bool flag = false;
	if (cmd[2][0] != '+' && cmd[2][0] != '-')
		return (sendMsg("no operator(+-) found\n", fd));
	if (cmd[2][0] == '+')
		flag = true;
	size_t j = 3;
	for (size_t i = 1; i < cmd[2].size(); i++)
	{
		if (cmd[2][i] == 'i') {
			if (flag)
				channel->setMode(INVITE_ONLY);
			else
				channel->unsetMode(INVITE_ONLY);
		}
		if (cmd[2][i] == 't') { 
			if (flag)
				channel->setMode(TOPIC_RESTRICTED);
			else
				channel->unsetMode(TOPIC_RESTRICTED);
		}
		if (cmd[2][i] == 'o') {
			if (cmd.size() >= (j + 1)) {
				if (flag)
				{
					std::cout << "entra a addOperator\n";
					channel->addOperator(findNickname(cmd[j], channel));
				 	std::cout << "para confirmar\n" << std::endl;//REVISAR que cliente es
				}
				else
				{
					std::cout << "entra a removeOperator\n";
					channel->removeOperator(findNickname(cmd[j], channel));
				} 
				j++;
			}
		}
		if (cmd[2][i] == 'k') {
			if (flag) {
				if (cmd.size() >= (j + 1)) { 
					channel->setPassword(cmd[j]);
					channel->setMode(PASSWORD_SET);
				}
				else
					sendMsg("no enviaste contrasenya bro", fd);
					//channel->setPassword("");
					//}
				j++;
			}
			else
				channel->unsetMode(PASSWORD_SET);
		}
		if (cmd[2][i] == 'l') {
			if (flag) {
				if (cmd.size() >= (j + 1)) {
					std::stringstream ss(cmd[j]);
					int num;
					ss >> num;
					channel->setUserLimit(num);
					channel->setMode(USER_LIMIT);
				}
			}
			else {
				channel->unsetMode(USER_LIMIT);
				channel->setUserLimit(MAX_CLIENTS); /// MIRAR BIEN CON EL RESTO DEL CODIGO
			}
			j++;
		}
	}
	std::cout << "Sale de Modemanagement" << std::endl;
}

void	Server::modeCmd(std::vector<string>& cmd, int fd)
{
	std::cout << "MODE cmd" << std::endl;
	printVecStr(cmd);
	if (cmd.size() < 2)
		return (sendMsg("no channel as arg ERR_NEEDMOREPARAMS (461) \n", fd));
	Channel*	channel = findChannel(cmd[1]);
	if (!isModeCmdValid(channel, cmd, fd))
		return ;
	if (!checkModeFlags(channel, cmd, fd))
		return ;
	// necesitamos checker si los modes son del mismo typo
	modeManagement(channel, cmd, fd);
}*/

// JOIN COMMAND
void	Server::joinCmd(std::vector<string>& cmd, int fd) {
	std::cout << "JOIN cmd" << std::endl;
	if (cmd.size() < 2)
		return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), cmd[0]), fd));

	std::vector<string> channelName = joinDivisor(cmd[1]);
	printVecStr(channelName);

	std::vector<string> channelPass;
	int pass = 0;
	if (cmd.size() > 2) {
		channelPass = joinDivisor(cmd[2]);
		printVecStr(channelPass);
		pass = channelPass.size();
	}
	// Check if channel exist
	Channel* found = NULL;
	int flag = 0;
	for (size_t i = 0; i < channelName.size(); i++) {
		if (validChannel(channelName[i], fd)) {
			found = channelsMng(channelName[i], fd);
			if (!found)
				createNewChannel(channelName[i], channelPass[i], pass, i, fd);
			else {
				if (!alreadyJoined(found, getClient(fd)->getUsername())) {
					std::cout << "not joined" << std::endl;
					if (channelPass.empty())
						flag = 1;
					existingChannel(found, channelPass[i], channelName[i], i, fd, flag);
				} else
					sendMsg("Already joined to the channel\r\n", fd);
			}
		}
	}
}

// PART COMMAND
void	Server::partCmd(std::vector<string>& cmd, int fd){
	std::cout << "PART cmd" << std::endl;
	Client*		client = getClient(fd);
	Channel*	channel;
	string		message;
	std::vector<string>	channelsVec;

	if (cmd.size() < 2)
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), cmd[0]), fd));
	
	channelsVec = joinDivisor(cmd[1]);
	std::cout << "channelsVec 0: " << channelsVec[0] << std::endl;
	for (size_t i = 0; i < channelsVec.size(); i++)
	{
		channel = findChannel(channelsVec[i]);
		if (!channel)
			return (sendMsg(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[0]), fd));
		if (!client->removeChannel(channelsVec[i]))
			return (sendMsg(ERR_NOTONCHANNEL(client->getNickname(), cmd[0]), fd));
		if (cmd.size() < 3)
			cmd.push_back("");
		message = "PART " + channelsVec[i] + " " + cmd[2];
		sendMsgToChannel(message, channel, fd);
		channel->removeClient(client->getUsername());
		if (channel->getClients().size() <= 0)
			removeChannel(channel->getName());
	}
}

//TOPIC COMMAND
void	Server::topicCmd(std::vector<string>& cmd, int fd){
	std::cout << "TOPIC cmd" << std::endl;
	if (cmd.size() == 2)
		topicDisplay(cmd[1], fd);
	if (cmd.size() > 2)
		topicSetter(cmd, fd);
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
/* 
// INVITE COMMAND
void	Server::inviteCmd(std::vector<string>& cmd, int fd){
	std::cout << "INVITE cmd" << std::endl;
	(void)cmd;
	(void)fd;
	}*/

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
