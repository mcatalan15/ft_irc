#include "../include/Server.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// CAP COMMAND
void	Server::capCmd(std::vector<std::string>& cmd, int fd) {
	if (cmd[1] == "LS")
		sendMsg("CAP * LS :0\r\n", fd);
	//std::cout << "message send" << std::endl;
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
		if (cmd[1] == getPassword())
			getClient(fd)->setState(NICK);
		else
			sendMsg(ERR_PASSWDMISMATCH(client->getNickname()), fd);
	}
}

// NICKNAME COMMAND
bool	Server::nickIsUsed(string cmd) {
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getNickname() == cmd)
			return false;
	}
	return true;
}

void	Server::nickCmd(std::vector<string>& cmd, int fd){
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
		}
	}
	return ;
}

// USER COMMAND
bool	Server::userIsUsed(string cmd) {
	// REALMENTE CUAL DEBERIA DE TODOS LOS NOMBRES DEBE MIRAR???
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getUsername() == cmd)
			return false;
	}
	return true;
}

void	Server::userCmd(std::vector<string>& cmd, int fd){
	Client*	client = getClient(fd);

	//printVecStr(cmd);
	if (cmd.size() < 2 || cmd[1].empty())
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), cmd[1]), fd));
	if (client->getState() == LOGIN && cmd.size() == 5) {
		if (cmd[1].size() > 18)
			cmd[1] = cmd[1].substr(0, 18);
		if (!userIsUsed(cmd[1]))
			sendMsg(ERR_USERNAMEINUSE(client->getUsername()), fd);
		else {
			client->setUsername(cmd[1]);
			client->setRealname(cmd[3]);
			client->setState(REGISTERED);
			client->welcome(*this, *getClient(fd), fd);
		}
	}
	else
		sendMsg(ERR_ALREADYREGISTERED(client->getNickname()), fd);
	return ;
}

// QUIT COMMAND
void	Server::quitCmd(std::vector<string>& cmd, int fd){
	std::cout << "QUIT cmd" << std::endl;

	string message;
	Client*	client = getClient(fd);
	std::vector<string>	channelsVec = client->getChannels();

	client->setMsg("");
	if (cmd.size() < 2)
		cmd.push_back("");
	message = "QUIT " + cmd[1];
	//std::cout << "sale de quit\n";
	for (size_t i = 0; i < channelsVec.size(); i++)
	{
		Channel*	channel = findChannel(channelsVec[i]);

		if (channel->hasClient(client->getUsername()))
			channel->removeClient(client->getUsername());
		if (channel->isOperator(client->getUsername()))
			channel->removeOperator(client->getUsername());
		channel->removeInvitation(client->getUsername());
		if (!channel->getClients().size())
			removeChannel(channelsVec[i]);
		else if (!channel->getOperators().size())
			channel->addOperator(channel->getClients()[0]);
	}
	sendMsgToClients(message, channelsVec, fd);
	channelsVec.clear();
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
	if (fd != _serverFd)
		close(fd);
}


// MODE COMMAND ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	Server::modeCmd(std::vector<string>& cmd, int fd)
{
	printVecStr(cmd);
    if (cmd.size() < 2)
		return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "MODE"), fd));

	Channel*	channel = findChannel(cmd[1]);

	if (!isModeCmdValid(channel, cmd, fd))
		return ;
	std::vector<string> modeChar = divisor(cmd[2], false);
	if (!checkModeFlags(modeChar, fd))
		return ;
	if (!validFlags(channel, modeChar, fd))
		return ;
	modeManagement(channel, cmd, modeChar, fd);
}

// JOIN COMMAND
void	Server::joinCmd(std::vector<string>& cmd, int fd) {
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
			found = channelsMng(channelName[i]);
			if (!found)
				createNewChannel(channelName[i], channelPass[i], pass, i, fd);
			else {
				if (!alreadyJoined(found, getClient(fd)->getUsername())) {
					if (channelPass.empty())
						flag = 1;
					existingChannel(found, channelPass[i], channelName[i], fd, flag);
				} else
					sendMsg(ERR_USERONCHANNEL(getClient(fd)->getNickname(), getClient(fd)->getNickname(), channelName[i]), fd);
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
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"), fd));

	channelsVec = joinDivisor(cmd[1]);
	for (size_t i = 0; i < channelsVec.size(); i++)
	{
		channel = findChannel(channelsVec[i]);
		if (!channel)
			return (sendMsg(ERR_NOSUCHCHANNEL(client->getNickname(), channelsVec[i]), fd));
		if (!client->removeChannel(channelsVec[i]))
			return (sendMsg(ERR_NOTONCHANNEL(client->getNickname(), channelsVec[i]), fd));
		if (cmd.size() < 3)
			cmd.push_back("");
		message = "PART " + channelsVec[i] + " " + cmd[2];
		sendMsgToChannel(message, channel, fd);
		channel->removeClient(client->getUsername());
		if (channel->isOperator(client->getUsername()))
			channel->removeOperator(client->getUsername());
		if (channel->getClients().size() <= 0)
			removeChannel(channel->getName());
		else if (!channel->getOperators().size())
			channel->addOperator(channel->getClients()[0]);
	}
}

//TOPIC COMMAND
void	Server::topicCmd(std::vector<string>& cmd, int fd){
	if (cmd.size() == 2)
		topicDisplay(cmd[1], fd);
	if (cmd.size() > 2)
		topicSetter(cmd, fd);
}

// KICK COMMAND
void	Server::kickCmd(std::vector<string>& cmd, int fd){
	std::cout << "KICK cmd" << std::endl;

	Client*			client = getClient(fd);
	Channel*		channel;
	std::vector<string>	clientsVec;

	if (cmd.size() < 3)
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"), fd));
	channel = findChannel(cmd[1]);
	if (!channel)
		return (sendMsg(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), fd));
	if (!channel->hasClient(client->getUsername()))
		return (sendMsg(ERR_NOTONCHANNEL(client->getNickname(), cmd[1]), fd));
	if (!channel->isOperator(client->getUsername()))
		return (sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickname(), cmd[1]), fd));
	if (cmd.size() < 4)
		cmd.push_back("");
	clientsVec = joinDivisor(cmd[2]);
	for (size_t i = 0; i < clientsVec.size(); i++)
	{
		string	message = "KICK " + cmd[1] + " " + clientsVec[i] + " " + cmd[3];
		Client*	kick = findNickname(clientsVec[i], channel);

		if (!kick)
			return (sendMsg(ERR_USERNOTINCHANNEL(client->getNickname(), clientsVec[i], channel->getName()), fd));
		if (channel->isOperator(kick->getUsername()))
			return (sendMsg(ERR_CANNOTKICK(client->getNickname(), clientsVec[i], channel->getName()), fd));

		kick->removeChannel(channel->getName());
		sendMsgToChannel(message, channel, fd);
		channel->removeClient(kick->getUsername());
		channel->addBannedClient(kick->getUsername());
	}
}

// PRIVMSG COMMAND
void	Server::privmsgCmd(std::vector<string>& cmd, int fd){
	std::cout << "PRIVMSG cmd" << std::endl;

	Client*		client = getClient(fd);
	Channel*	channel = NULL;
	Client*		user = NULL;
	string		message;
	std::vector<string>	destinationVec;

	if (cmd.size() < 3)
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"), fd));
	destinationVec = joinDivisor(cmd[1]);
	for (size_t i = 0; i < destinationVec.size(); i++)
	{
		channel = findChannel(destinationVec[i]);
		user = getNick(destinationVec[i]);
		message = "PRIVMSG " + destinationVec[i] + " " + cmd[2];

		if (channel)
			sendMsgToChannel(message, channel, fd);
		else if (user)
			sendMsg(USER_ID(client->getNickname(), client->getUsername()) + " " + message + CRLF, user->getFd());
		else
			return (sendMsg(ERR_NOSUCHCHANNELORCLIENT(client->getNickname(), destinationVec[i]), fd));
	}
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
	(void)cmd;
	(void)fd;

	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "╔════════════════════════════════════════╗"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║      Welcome to FT_IRC Server      ║"), fd);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║         Powered by eferr-m jpaul-kr mcatalan         ║"), fd);
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
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), "║  Visit:           ║"), fd);
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
