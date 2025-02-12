#include "../include/Server.hpp"
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

Channel* Server::channelsMng(string& channelName, int fd) {
	(void)fd; // WTF
	for (size_t j = 0; j < _channels.size(); j++)
	{
		if (_channels[j].getName() == channelName)
			return &_channels[j];
	}
	return NULL;
}

void	Server::joinMsg(Channel *channel, int fd) {
	std::vector<string> usersList = channel->getClients();
	string msg = "";
	for (size_t i = 0; i < usersList.size(); i++) {
		msg.append(" ");
		if (channel->isOperator(usersList[i]))
			msg.append("@");
		msg.append(getUser(usersList[i])->getNickname());
	}
	std::cout << msg << std::endl;
	string msg2 = "JOIN :";
	msg2.append(channel->getName());
	sendMsgToChannel(msg2 , channel, fd);
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
	if (i < pass) {
		newchannel.setPassword(channelPass);
		newchannel.setMode(PASSWORD_SET);
	}
	newchannel.addClient(username);
	newchannel.addOperator(username);
	newchannel.setTopic("");
	_channels.push_back(newchannel);
	getClient(fd)->addChannel(channelName);
	joinMsg(&newchannel, fd);
}

bool	Server::channelConnStatus(int fd, Channel *found, string& channelPass, string& channelName) {
	(void)channelPass;
	if (found->isBanned(getClient(fd)->getUsername())) { //User is banned from channel
		sendMsg(ERR_BANNEDFROMCHAN(getClient(fd)->getNickname(), channelName), fd);
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
		if (found->isInvited(getClient(fd)->getUsername())) {
			std::cout << "Esta invitado" << getClient(fd)->getNickname() << " " << channelName << std::endl;
			return true;
		}
		else {
			std::cout << "NO invitado" << getClient(fd)->getNickname() << " " << channelName << std::endl;
			sendMsg(ERR_INVITEONLYCHAN(getClient(fd)->getNickname(), channelName), fd);
			return false;
		}
	}
	return true;
}

void	Server::existingChannel(Channel* found, string& channelPass, string& channelName,int i, int fd, int flag) {
	(void)i; //WTF
	if (found->isModeSet(PASSWORD_SET)) {
		if (flag == 1) {
			string channelPass = "";
			sendMsg("bro no pusiste key y se necesita XD\n", fd);
			return ;
		}
		if (channelPass == found->getPassword()) {
			if (channelConnStatus(fd, found, channelPass, channelName)) {
				found->addClient(getClient(fd)->getUsername());
				getClient(fd)->addChannel(found->getName());
				joinMsg(found, fd);
			}
		}
		else
			sendMsg(ERR_BADCHANNELKEY(getClient(fd)->getNickname(), channelName), fd);
	}
	else {
		if (channelConnStatus(fd, found, channelPass, channelName)) {
			found->addClient(getClient(fd)->getUsername());
			getClient(fd)->addChannel(found->getName());
			joinMsg(found, fd);
		}
	}
}
