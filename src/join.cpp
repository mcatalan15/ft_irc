#include "../include/Server.hpp"
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
/*
std::vector<string>	joinDivisor(string cmd) {

	size_t	i;
	size_t	init = 0;
	std::vector<string>	vec;
	int		num_ch = 0;
	for (i = 0; i < cmd.size(); i++) {
		if (cmd[i] == ',')
		{
			vec.push_back(cmd.substr(init, i - init));
			init = i + 1;
			num_ch++;
		}
	}
	vec.push_back(cmd.substr(init, -1));
	return vec;
	}*/

//bool	Server::channelIsFull(Channel* channel, int fd)

Channel* Server::channelsMng(string& channelName, int fd) {
	(void)fd;
	for (size_t j = 0; j < _channels.size(); j++)
	{
		if (_channels[j].getName() == channelName)
			return &_channels[j];
	}
	return NULL;
}

void	Server::joinMsg(Channel *channel, int fd) {
	std::vector<string> usersList = channel->getClients();
	sendMsg(RPL_CONNECT(getClient(fd)->getNickname(), getClient(fd)->getUsername(), channel->getName()), fd);
	string msg = "";
	for (size_t i = 0; i < usersList.size(); i++) {
		msg.append(" ");
		if (channel->isOperator(usersList[i]))
			msg.append("@");
		msg.append(getUser(usersList[i])->getNickname()); // cambiar a nick (es user)
	}
	std::cout << msg << std::endl;
	sendMsg(RPL_NAMREPLY(getClient(fd)->getNickname(), channel->getName(), msg), fd); //RPL_NAMREPLY 353
	sendMsg(RPL_ENDOFNAMES(getClient(fd)->getNickname(), channel->getName()), fd); //RPL_ENDOFNAMES 366
	sendMsg(RPL_TOPIC(getClient(fd)->getNickname(), channel->getName(), "EMTPY TO-DO TOPIC"), fd);
	sendMsg(RPL_TOPICWHOTIME(getClient(fd)->getNickname(), channel->getName(), getClient(fd)->getNickname(), channel->getCreationTime()), fd);
}

void	Server::createNewChannel(string& channelName, string& channelPass, int pass, int i, int fd) {
	Channel newchannel(channelName);
	string	username = getClient(fd)->getUsername();

	if (_channels.size() >= MAX_CHANNELS) //User joins max channels
			return (sendMsg(ERR_TOOMANYCHANNELSCREATED(getClient(fd)->getNickname(), channelName), fd));
	if (getClient(fd)->clientMaxChannel()) //User joins max channels
		return (sendMsg(ERR_TOOMANYCHANNELS(getClient(fd)->getNickname(), channelName), fd));
	if (i < pass)
	{
		std::cout << "channel: " << channelName << "  hay password: " << channelPass << std::endl;
		newchannel.setPassword(channelPass);
		newchannel.setMode(PASSWORD_SET);
	}
	else
		std::cout << "channel: " << channelName << "  NO password" << std::endl;
	newchannel.addClient(username);
	newchannel.addOperator(username);
	_channels.push_back(newchannel);
	getClient(fd)->addChannel(channelName);
	//std::cout << "name channel: " << newchannel.getName() << std::endl;
	//std::cout << "channels server addr: " << &_channels[0] << std::endl;
	//std::cout << "channels client addr: " << getClient(fd)->getChannels()[0] << std::endl;
	sendMsg("channel " + channelName + " created!\n", fd);
	joinMsg(&newchannel, fd);
}

bool	Server::channelConnStatus(int fd, Channel *found, string& channelPass, string& channelName) {
	(void)channelPass;
	if (found->isBanned(getClient(fd)->getUsername())) { //User is banned from channel
		std::cout << "IS baned" <<std::endl;
		return false;
	}
	if (getClient(fd)->clientMaxChannel()) { //User joins max channels
		sendMsg(ERR_TOOMANYCHANNELS(getClient(fd)->getNickname(), channelName), fd);
		return false;
	}
	if (found->isFull()) { //Channel is full
		sendMsg(ERR_CHANNELISFULL(getClient(fd)->getNickname(), channelName), fd);
		return false;
	}
	if (found->isModeSet(INVITE_ONLY)) { // IF channel is invite only
		sendMsg(ERR_INVITEONLYCHAN(getClient(fd)->getNickname(), channelName), fd);
		return false;
	}
	else if (found->isInvited(getClient(fd)->getUsername())) {
		std::cout << "Esta invitado" << getClient(fd)->getNickname() << " " << channelName << std::endl;
		return true;
	}
	else 
		std::cout << "NO invitado" << getClient(fd)->getNickname() << " " << channelName << std::endl;
	return true;
}

void	Server::existingChannel(Channel* found, string& channelPass, string& channelName, int i, int fd, int flag) {
	std::cout << "Existing channel" << std::endl;
	if (found->isModeSet(PASSWORD_SET))
	{
		std::cout << "Entra a tiene contrasenya\n";
		std::cout << "i: " << i << std::endl;
		if (flag == 1) {
			string channelPass = "";
			sendMsg("bro no pusiste key y se necesita XD\n", fd);
			return ;
		}
		if (channelPass == found->getPassword())
		{
			if (channelConnStatus(fd, found, channelPass, channelName))
			{
				found->addClient(getClient(fd)->getUsername());
				getClient(fd)->addChannel(found->getName());
				//sendMsg(RPL_TOPIC(getClient(fd)->getNickname(), channelName, "EMTPY TO-DO TOPIC"), fd);
				//sendMsg(RPL_TOPICWHOTIME(getClient(fd)->getNickname(), channelName, getClient(fd)->getNickname(), "EMPTY TO-DO hora de creacion"), fd);
				joinMsg(found, fd);
			}
		}
		else
			sendMsg(ERR_BADCHANNELKEY(getClient(fd)->getNickname(), channelName), fd);
	}
	else
	{
		if (channelConnStatus(fd, found, channelPass, channelName))
		{
			found->addClient(getClient(fd)->getUsername());
			getClient(fd)->addChannel(found->getName());
			//sendMsg(RPL_TOPIC(getClient(fd)->getNickname(), channelName, "EMTPY TO-DO TOPIC"), fd);
			//sendMsg(RPL_TOPICWHOTIME(getClient(fd)->getNickname(), channelName, getClient(fd)->getNickname(), "EMPTY TO-DO hora de creacion"), fd);
			joinMsg(found, fd);
		}
	}
		std::cout << "no tiee pass" << std::endl;
}

bool validChannel(string& channelName, int fd) {
	if (channelName[0] == '#')
		return true;
	sendMsg(ERR_BADCHANMASK(channelName), fd);
	return false;
}

void	Server::joinCmd(std::vector<string>& cmd, int fd) {
	std::cout << "JOIN cmd" << std::endl;
	Client*		client = getClient(fd);
	if (cmd.size() < 2)
		return (sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), cmd[0]), fd));

	std::cout << "CANALES!!!" << std::endl;
	std::vector<string> channelName = joinDivisor(cmd[1]);
	printVecStr(channelName);

	std::vector<string> channelPass;
	int pass = 0;
	if (cmd.size() > 2) {
		std::cout << std::endl << "KEYYSSS!!!!" <<std::endl;
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
				if (channelPass.empty())
					flag = 1;
				existingChannel(found, channelPass[i], channelName[i], i, fd, flag);
			}
		}
	}
}
