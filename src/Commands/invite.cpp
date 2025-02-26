#include "../../include/Server.hpp"

bool	Server::isInviteCmdValid(Channel* channel, std::vector<string>& cmd, int fd)
{
	if (!channel)
	{
		sendMsg(ERR_NOSUCHCHANNEL(getClient(fd)->getUsername(), cmd[2]), fd);
		return (false);
	}
	if (!channel->hasClient(getClient(fd)->getUsername()))
	{
		sendMsg(ERR_NOTONCHANNEL(getClient(fd)->getUsername(), cmd[2]), fd);
		return (false);
	}
	if (channel->isModeSet(INVITE_ONLY)) {
		if (!channel->isOperator(getClient(fd)->getUsername()))
		{
			sendMsg(ERR_CHANOPRIVSNEEDED(getClient(fd)->getUsername(), cmd[2]), fd);
			return (false);
		}
	}
	return (true);
}

std::vector<string>	Server::divisor(string cmd, bool flag)
{
	size_t	i;
	size_t	init;
	std::vector<string>	vec;
	int		num_ch = 0;

	flag ? i = 0 : i = 1;
	init = i;
	while (i < cmd.size())
	{
		if (cmd[i] == ',')
		{
			vec.push_back(cmd.substr(init, i - init));
			init = i + 1;
			num_ch++;
		}
		i++;
	}
	vec.push_back(cmd.substr(init, -1));
	return vec;
}

bool	Server::findNicknameOnServer(string nickName, int fd)
{
	std::vector<Client>&	lstClients = getClients();

	for (size_t i = 0; i < lstClients.size(); i++)
	{
		if (lstClients[i].getNickname() == nickName)
			return (true);
	}
	sendMsg(ERR_NOSUCHNICK(getClient(fd)->getUsername(), nickName), fd);
	return (false);
}

bool	Server::nicknameExist(std::vector<string> nickName, int fd)
{
	for (size_t i = 0; i < nickName.size(); i++)
	{
		if (!findNicknameOnServer(nickName[i], fd))
			return (false);
	}
	return (true);
}

bool	Server::userOnChannel(Channel* channel, std::vector<string> nickName, int fd)
{
	for (size_t i = 0; i < nickName.size(); i++)
	{
		if (channel->hasClient(nickName[i]))
		{
			sendMsg(ERR_USERONCHANNEL(getClient(fd)->getNickname(), nickName[i], channel->getName()), fd);
			return (true);
		}
	}
	return (false);
}

void    Server::sendInvitationMsg(Channel *channel, string nickName, int fd)
{
    Client* user = getNick(nickName);

    sendMsg(INVITE_MESSAGE(getClient(fd)->getNickname(), getClient(fd)->getUsername(), nickName, channel->getName()), user->getFd());
}

void	Server::invitationManagement(Channel* channel, std::vector<string>& nickName, int fd, bool flag)
{
    for (size_t i = 0; i < nickName.size(); i++)
	{
		Client*	client = getNick(nickName[i]);
		if (flag)
		{
			channel->addInvitation(client->getUsername());
			sendMsg(RPL_INVITING(getClient(fd)->getNickname(), nickName[i], channel->getName()), fd); //RPL_INVITING (341)
			sendInvitationMsg(channel, nickName[i], fd);
		}
		else
		{
			if (channel->isInvited(client->getUsername()))
				channel->removeInvitation(client->getUsername());
		}
	}
}
