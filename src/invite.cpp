#include "../include/Server.hpp"
#include "../include/Channel.hpp"

/*
RPL_INVITING (341)
ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442) // tengo que verificarlo
ERR_CHANOPRIVSNEEDED (482)
ERR_USERONCHANNEL (443)
*/

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
    // Hay que verificar al mismo tiempo si es un canal solo por invitacion
    // si es solo por invitacion verificar si es opé sino da igual
	if (!channel->isOperator(getClient(fd)->getUsername()))
	{
	   sendMsg(ERR_CHANOPRIVSNEEDED(getClient(fd)->getUsername(), cmd[2]), fd);
		return (false);
	}
	return (true);
}

std::vector<string>	Server::divisor(string cmd, bool flag)
{
	size_t	i;
	size_t	init = 0;
	std::vector<string>	vec;
	int		num_ch = 0;

	flag ? i = 0 : i = 1;
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
	sendMsg(ERR_NOSUCHNICK(getClient(fd)->getUsername(), nickName), fd);// Hay que enviar un mensage si el nickname no existe ??
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

void	Server::invitationManagement(Channel* channel, std::vector<string>& nickName, int fd, bool flag)
{
	for (size_t i = 0; i < nickName.size(); i++)
	{
		if (flag)
		{
			channel->addInvitation(nickName[i]);
			sendMsg(RPL_INVITING(getClient(fd)->getNickname(), nickName[i], channel->getName()), fd); //RPL_INVITING (341)
			// enviar un msg a la persona invitada
		}
		else
		{
			if (channel->isInvited(nickName[i]))
				channel->removeInvitation(nickName[i]);
		}
	}
}

void	Server::inviteCmd(std::vector<string>& cmd, int fd)
{
 	std::cout << "INVITE cmd" << std::endl;
	string command = cmd[0];

	if (cmd.size() < 3)
		return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), command), fd));

	Channel*	channel = findChannel(cmd[2]);
	if (!isInviteCmdValid(channel, cmd, fd))
		return ;
	bool flag = (cmd[1][0] != '-' ? true : false);
	//bool flag = isPositif(cmd[1]);
	std::vector<string> nickName = divisor(cmd[1], flag);
	if (!nicknameExist(nickName, fd))
		return ;
	if (userOnChannel(channel, nickName, fd))
	   return ;
	invitationManagement(channel, nickName, fd, flag);
	sendMsg(RPL_ENDOFINVITELIST(getClient(fd)->getNickname()), fd);
}
