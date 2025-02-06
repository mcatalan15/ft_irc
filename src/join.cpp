#include "../include/Server.hpp"
#include <iostream>
#include <ostream>

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
}

//bool	Server::channelIsFull(Channel* channel, int fd)

Channel* Server::channelsMng(string& channelName, int fd) {
	(void)fd;
	Channel* found = NULL;
	for (size_t j = 0; j < _channels.size(); j++)
	{
		if (_channels[j].getName() == channelName)
			found = &_channels[j];
	}
	return found;
}

void	Server::createNewChannel(string& channelName, string& channelPass, int pass, int i, int fd) {
	std::cout << "Entra" << std::endl;
	Channel newchannel(channelName);
	if (i < pass) {
		std::cout << "channel: " << channelName << "  hay password: " << channelPass << std::endl;
		newchannel.setPassword(channelPass);
	}
	else
		std::cout << "channel: " << channelName << "  NO password" << std::endl;
	newchannel.addClient(getClient(fd));
	newchannel.addOperator(getClient(fd));
	_channels.push_back(newchannel);
	getClient(fd)->addChannel(&newchannel);
	sendMsg("channel " + channelName + " created!\n", fd);
}

void	Server::existingChannel(Channel* found, string& channelPass, string& channelName, int i, int fd) {
	// mierdas de mode y password
	if (found->hasPassword()) {
		std::cout << "i: " << i << std::endl;
		if (channelPass == found->getPassword()) {
			if ()
			found->addClient(getClient(fd));
			getClient(fd)->addChannel(found);
			sendMsg(RPL_TOPIC(getClient(fd)->getNickname(), channelName, "EMTPY TO-DO TOPIC"), fd);
			sendMsg(RPL_TOPICWHOTIME(getClient(fd)->getNickname(), channelName, getClient(fd)->getNickname(), "EMPTY TO-DO hora de creacion"), fd);
			
			// En caso inite only channel para unirse ERR_INVITEONLYCHAN
			// USUARIO se une a demasiados canales ERR_TOOMANYCHANNELS
			// USUARIO BANEADO ERR_BANNEDFROMCHAN 
			// CHANNEL FULL ERR_CHANNELISFULL
		}
		else
			sendMsg(ERR_BADCHANNELKEY(getClient(fd)->getNickname(), channelName), fd);
	}
	std::cout << "no tiee ppass" << std::endl;
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
	std::vector<string> channelsNames = joinDivisor(cmd[1]);
	printVecStr(channelsNames);
	
	std::vector<string> channelsPass;
	int pass = 0;
	if (cmd.size() > 2) {
		std::cout << std::endl << "KEYYSSS!!!!" <<std::endl;
		channelsPass = joinDivisor(cmd[2]);
		printVecStr(channelsPass);
		pass = channelsPass.size();
	}

	// Check if channel exist
	Channel* found = NULL;
	for (size_t i = 0; i < channelsNames.size(); i++) {
		if (validChannel(channelsNames[i], fd)) {
			found = channelsMng(channelsNames[i], fd);
			if (!found)
				createNewChannel(channelsNames[i], channelsPass[i], pass, i, fd);
			else
				existingChannel(found, channelsPass[i], channelsNames[i], i, fd);
		}
	}
}
