#include "../include/Server.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// CAP COMMAND
void	Server::capCmd(std::vector<std::string>& cmd, int fd) {
	if (cmd[1] == "LS")
		sendMsg("CAP * LS :0\r\n", fd);
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
			sendMsg(ERR_ERRONEUSNICKNAME(cmd[1]), fd);
		else if (!nickIsUsed(cmd[1]))
			sendMsg(ERR_NICKNAMEINUSE(cmd[1]), fd);
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
		sendMsg(ERR_ALREADYREGISTERED(client->getNickname()), fd); //???
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
	else
		cmd[1] = ":" + cmd[1];
	message = "QUIT " + cmd[1];
	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << _clients[i].getNickname() << ": \n";
		for (size_t j = 0; j < _clients[i].getChannels().size(); j++)
		{
			if (!_clients[i].getChannels().empty())
				std::cout << _clients[i].getChannels()[j] << std::endl;
		}
	}
	sendMsgToClients(message, channelsVec, fd);
	for (size_t i = 0; i < channelsVec.size(); i++)
	{
		std::cout << "entra XD" << std::endl;
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
	// std::cout << "channels num: " << _channels.size() << std::endl; 
	
	channelsVec.clear();
	// std::cout << "client name: " << _clients[1].getUsername() << std::endl; 
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->getFd() == fd) {
			it = _clients.erase(it);
			break;
		}
	}
	for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); it++) {
		if (it->fd == fd) {
			it = _pollFds.erase(it);
			break;
		}
	}
	if (fd != _serverFd)
		close(fd);
}


// MODE COMMAND ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	Server::modeCmd(std::vector<string>& cmd, int fd)
{
	//printVecStr(cmd);
	if (getClient(fd)->getState() != REGISTERED)
		sendMsg(ERR_NOTREGISTERED(getClient(fd)->getHostname()), fd);
	
    if (cmd.size() < 2)
		return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "MODE"), fd));

	Channel*	channel = findChannel(cmd[1]);

	if (!isModeCmdValid(channel, cmd, fd))
		return ;
	std::vector<string> vec = divisor(cmd[2], false);
	string modeChar = vec[0];
	if (!checkModeFlags(modeChar, fd))
		return ;
	bool sign = (cmd[2][0] != '+' ? false : true);
	if (!validFlags(channel, modeChar, fd, sign))
		return ;
	modeManagement(channel, cmd, modeChar, fd);
}

// JOIN COMMAND
void	Server::joinCmd(std::vector<string>& cmd, int fd) {
	if (getClient(fd)->getState() == REGISTERED) {
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
	} else
	sendMsg(ERR_NOTREGISTERED(getClient(fd)->getHostname()), fd);
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
	printVecStr(cmd);
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
		else
			cmd[2] = ":" + cmd[2];
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
	if (getClient(fd)->getState() == REGISTERED) {
		if (cmd.size() == 2)
			topicDisplay(cmd[1], fd);
		else if (cmd.size() > 2)
			topicSetter(cmd, fd);
		else
			sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "TOPIC"), fd);
	} else
		sendMsg(ERR_NOTREGISTERED(getClient(fd)->getHostname()), fd);
}

// KICK COMMAND
void	Server::kickCmd(std::vector<string>& cmd, int fd){
	std::cout << "KICK cmd" << std::endl;

	Client*			client = getClient(fd);
	Channel*		channel;
	std::vector<string>	clientsVec;

	printVecStr(cmd);
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
	else
		cmd[3] = ":" + cmd[3];
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

	printVecStr(cmd);
	if (cmd.size() < 3)
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"), fd));
	//std::cout << cmd[2] << std::endl;
	destinationVec = joinDivisor(cmd[1]);
	for (size_t i = 0; i < destinationVec.size(); i++)
	{
		channel = findChannel(destinationVec[i]);
		user = getNick(destinationVec[i]);
		message = "PRIVMSG " + destinationVec[i] + " :" + cmd[2];

		if (channel)
			sendMsgToChannel(message, channel, fd);
		else if (user)
			sendMsg(USER_ID(client->getNickname(), client->getUsername()) + " " + message + CRLF, user->getFd());
		else
			return (sendMsg(ERR_NOSUCHCHANNELORCLIENT(client->getNickname(), destinationVec[i]), fd));
	}
}

// INVITE COMMAND
void	Server::inviteCmd(std::vector<string>& cmd, int fd)
{
    std::cout << "INVITE cmd" << std::endl;
	string command = cmd[0];
	
	if (getClient(fd)->getState() != REGISTERED)
		sendMsg(ERR_NOTREGISTERED(getClient(fd)->getHostname()), fd);
	
	if (cmd.size() < 3)
		return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), command), fd));

	Channel*	channel = findChannel(cmd[2]);
	if (!isInviteCmdValid(channel, cmd, fd))
		return ;
	bool flag = (cmd[1][0] != '-' ? true : false);
	std::vector<string> nickName = divisor(cmd[1], flag);
	if (!nicknameExist(nickName, fd))
		return ;
	if (userOnChannel(channel, nickName, fd))
	   return ;
	invitationManagement(channel, nickName, fd, flag);
	sendMsg(RPL_ENDOFINVITELIST(getClient(fd)->getNickname()), fd);
}

// INFO COMMAND
void	Server::infoCmd(std::vector<string>& cmd, int fd){
	(void)cmd;
	(void)fd;
	// Create the vector of strings
	std::vector<string>	infoLines = getInfo();

	// Check for max len
	size_t maxLen = 0;
	for (size_t line = 0; line < infoLines.size(); line++) {
		if (infoLines[line].length() > maxLen)
			maxLen = infoLines[line].size();
	}
	
	std::string upBorder = string(UP_LEFT) + horizontalChars(maxLen + 2) + string(UP_RIGHT);
	std::string downBorder = string(DOWN_LEFT) + horizontalChars(maxLen + 2) + string(DOWN_RIGHT);
	std::string middleBorder = string(MIDDLE_LEFT) + horizontalChars(maxLen + 2) + string(MIDDLE_RIGHT);
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), upBorder),fd);
	for (size_t i = 0; i < infoLines.size(); i++) {
		string line = infoLines[i];
		if (infoLines[i] == "line")
			sendMsg(RPL_INFO(getClient(fd)->getNickname(), middleBorder), fd);
		else if (i == 0 || i == 1) {
			line = centerText(line, maxLen);
			sendMsg(RPL_INFO(getClient(fd)->getNickname(), createTableRow(line, maxLen + 2)), fd);
		}
		else
			sendMsg(RPL_INFO(getClient(fd)->getNickname(), createTableRow(line, maxLen + 2)), fd);
	}
	sendMsg(RPL_INFO(getClient(fd)->getNickname(), downBorder),fd);
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


// BONUS
// DCC cmd
//void	Server::dccCmd(std::vector<string>& cmd, inf fd) {
	
//}
