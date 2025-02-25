#include "../../include/Server.hpp"

Channel* Server::channelsMng(string& channelName) {
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
	std::cout << channel->getCreationTime() << std::endl;
	msg2.append(channel->getName());
	sendMsgToChannel(msg2 , channel, fd);
	sendMsg(RPL_NAMREPLY(getClient(fd)->getNickname(), channel->getName(), msg), fd); //RPL_NAMREPLY 353
	sendMsg(RPL_ENDOFNAMES(getClient(fd)->getNickname(), channel->getName()), fd); //RPL_ENDOFNAMES 366
	if (channel->getTopic().empty()) // No topic set
		sendMsg(RPL_NOTOPIC(getClient(fd)->getNickname(), channel->getName()), fd);
	else // Topic set
		sendMsg(RPL_TOPIC(getClient(fd)->getNickname(), channel->getName(), channel->getTopic()), fd);
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
	(void)channelPass; // Unused parameter
	// Check if the user is banned from the channel
	if (found->isBanned(getClient(fd)->getUsername())) {
		sendMsg(ERR_BANNEDFROMCHAN(getClient(fd)->getNickname(), channelName), fd);
		return false;
	}

	// Check if the user has joined too many channels
	if (getClient(fd)->clientMaxChannel()) {
		sendMsg(ERR_TOOMANYCHANNELS(getClient(fd)->getNickname(), channelName), fd);
		return false;
	}

	// Check if the channel is full
	if (found->isFull()) {
		sendMsg(ERR_CHANNELISFULL(getClient(fd)->getNickname(), channelName), fd);
		return false;
	}

	// Check if the channel is invite-only
	if (found->isModeSet(INVITE_ONLY) && !found->isInvited(getClient(fd)->getUsername())) {
		sendMsg(ERR_INVITEONLYCHAN(getClient(fd)->getNickname(), channelName), fd);
		return false;
	}
	return true;
}

void	Server::existingChannel(Channel* found, string& channelPass, string& channelName, int fd, int flag) {
	if (found->isModeSet(PASSWORD_SET)) {
		if (flag == 1 || channelPass != found->getPassword()) {
			sendMsg(ERR_BADCHANNELKEY(getClient(fd)->getNickname(), channelName), fd);
			return;
		}
	}

	if (!channelConnStatus(fd, found, channelPass, channelName))
		return;

	found->addClient(getClient(fd)->getUsername());
	getClient(fd)->addChannel(found->getName());
	joinMsg(found, fd);
}
