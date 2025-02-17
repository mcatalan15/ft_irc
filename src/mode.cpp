#include "../include/Server.hpp"
#include "../include/Channel.hpp"
#include <string>

bool	Server::validFlags(Channel* channel, std::vector<string>& cmd, int fd)
{
	(void) channel;
	(void) fd;
	std::vector<bool> charactersRepeat(128, false);

	for (std::string::iterator it = cmd[2].begin(); it != cmd[2].end(); it++)
	{
		char c = *it;
		if (charactersRepeat[static_cast<unsigned char>(c)])
		{
			sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getNickname(), cmd[1], cmd[2], c, "A Mode Operator is repeat"), fd);
            return (false);
		}
        charactersRepeat[static_cast<unsigned char>(c)] = true;
    }
    return (true);
}

bool	Server::isFlagMode(Channel* channel, std::vector<string>& cmd, int num, int fd)
{
	(void)channel;
	if (num == 1)
	{
		for (size_t i = 1; i < cmd[2].size(); i++)
		{
			if (cmd[2].find_first_of(CHANNEL_MODES) == string::npos)
			{
				sendMsg(ERR_UMODEUNKOWNFLAG(getClient(fd)->getUsername()), fd);
				return (false);
			}
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
	if (!isFlagMode(channel, cmd, 2, fd))
	{
		if (!isFlagMode(channel, cmd, 1, fd))
			return (false);
		return (true);
	}
	return (true);
}

bool	Server::isModeCmdValid(Channel* channel, std::vector<string>& cmd, int fd)
{
	if (!channel)
	{
		sendMsg(ERR_NOSUCHCHANNEL(getClient(fd)->getUsername(), cmd[1]), fd);
		return (false);
	}
	if (cmd.size() < 3)
	{
		sendMsg(RPL_CREATIONTIME(getClient(fd)->getUsername(), cmd[1], getCreationTime()), fd);
		return(false);
	}
	if (!channel->isOperator(getClient(fd)->getUsername())) {
		sendMsg(ERR_CHANOPRIVSNEEDED(getClient(fd)->getUsername(), cmd[1]), fd);
		return (false);
	}
	if (cmd[2][0] != '+' && cmd[2][0] != '-'){

		sendMsg(ERR_UMODEUNKOWNFLAG(getClient(fd)->getUsername()), fd);
		return (false);
	}
	return (true);
}

/*Client*	Server::findNickname(string nick, Channel* channel)
{
	const std::vector<string>&	lstClients = channel->getClients();
	std::cout << "findNickname: " << std::endl;
	(void)nick;
	(void)channel;
	if (lstClients.size() <= 0) {
		std::cout << "No hay clients en este channel" << std::endl;
		return NULL;
	}
	for (size_t i = 0; i < lstClients.size(); i++)
	{
		if (getUser(lstClients[i])->getNickname() == nick)
			return getUser(lstClients[i]);
	}
	return NULL;
	}*/

bool Server::isNumber(string cmd)
{
    for (size_t i = 1; i <= cmd.size(); i++)
	{
		if (cmd.find_first_of("0123456789") == string::npos)
			return (false);
	}
	return (true);
}

void	Server::flagModeL(bool flag, Channel* channel, string cmd)
{
	if (flag)
	{
		std::stringstream ss(cmd);
		int num;
		ss >> num;
		if (num > MAX_CLIENTS)
			num = MAX_CLIENTS;
			// WE NEED TO SEND A MESSAGE FOR RHAT ??
		channel->setUserLimit(num);
		channel->setMode(USER_LIMIT);
	}
	else
	{
		channel->unsetMode(USER_LIMIT);
		channel->setUserLimit(MAX_CLIENTS); /// MIRAR BIEN CON EL RESTO DEL CODIGO
	}
}

void	Server::flagModeK(bool flag, Channel* channel, std::vector<string>& cmd, int fd)
{
	if (flag)
	{
		//check password if it's invalid (ERR_INVALIDKEY (525))
		//if (key is invalid)
		//	return (ERR_INVALIDKEY)
		if (channel->hasPassword())
			return (sendMsg(ERR_KEYSET(cmd[1]), fd));
		channel->setPassword(cmd[3]);
		channel->setMode(PASSWORD_SET);
	}
	else
	{
		channel->getPassword().clear();
		channel->unsetMode(PASSWORD_SET);
	}
}

void    Server::sendModeMsg(Channel *channel, string s1, string target, int fd)
{
    Client* client = getClient(fd);
    string  channelName = channel->getName();
    std::vector<string>	clientsVec = channel->getClients();

    for (size_t i = 0; i < clientsVec.size(); i++)
        sendMsg(MODE_MESSAGE(client->getNickname(), client->getUsername(), channelName, s1, target), getUser(clientsVec[i])->getFd());
}

void	Server::flagModeO(bool flag, Channel* channel, string cmd, int fd)
{
    if (flag)
	{
		channel->addOperator(findNickname(cmd, channel)->getUsername());
	 	sendModeMsg(channel, "+o", cmd, fd);
	}
	else
	{
		channel->removeOperator(findNickname(cmd, channel)->getUsername());
		sendModeMsg(channel, "-o", cmd, fd);
	}
}

void	Server::flagModeT(bool flag, Channel *channel)
{
	if (flag)
		channel->setMode(TOPIC_RESTRICTED);
	else
		channel->unsetMode(TOPIC_RESTRICTED);
}

void	Server::flagModeI(bool flag, Channel *channel)
{
	if (flag)
		channel->setMode(INVITE_ONLY);
	else
		channel->unsetMode(INVITE_ONLY);
}

void	Server::modeManagement(Channel* channel, std::vector<string>& cmd, int fd)
{
	bool flag = false;
	if (cmd[2][0] == '+')
		flag = true;
	size_t j = 3;
	for (size_t i = 1; i < cmd[2].size(); i++)
	{
		if (cmd[2][i] == 'i')
			flagModeI(flag, channel);
		if (cmd[2][i] == 't')
			flagModeT(flag, channel);
		if (cmd[2][i] == 'o')
		{
			if (cmd.size() >= (j + 1))
			{
				if (!channel->hasClient(cmd[j]))
					return(sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), cmd[1], cmd[2], cmd[3], "Is not on that channel"), fd));
				flagModeO(flag, channel, cmd[j], fd);
				j++;
			}
			else
				return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getUsername(), cmd[2]), fd));
		}
		if (cmd[2][i] == 'k') {
			if (flag)
			{
				if (cmd.size() >= (j + 1))
				{
					flagModeK(flag, channel, cmd, fd);
					j++;
				}
				else
					return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getUsername(), cmd[2]), fd));
			}
			else
				flagModeK(flag, channel, cmd, fd);
		}
		if (cmd[2][i] == 'l') {
			if (flag)
			{
				if (cmd.size() >= (j + 1))
				{
					if (!isNumber(cmd[j]))
						return (sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), cmd[1], cmd[2], cmd[3], "Invalid limit"), fd));
					flagModeL(flag, channel, cmd[j]);
					j++;
				}
				else
					return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getUsername(), cmd[2]), fd));
			}
			else
				flagModeL(flag, channel, cmd[j]);
		}
	}
}

void	Server::modeCmd(std::vector<string>& cmd, int fd)
{
	std::cout << "MODE cmd" << std::endl;
	printVecStr(cmd);
	if (cmd.size() < 2)
		return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "MODE"), fd));
	Channel*	channel = findChannel(cmd[1]);
	if (!isModeCmdValid(channel, cmd, fd))
		return ;
	if (!checkModeFlags(channel, cmd, fd))
		return ;
	if (!validFlags(channel, cmd, fd))
		return ;
	modeManagement(channel, cmd, fd);
}
