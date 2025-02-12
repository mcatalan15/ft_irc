#include "../include/Server.hpp"
#include "../include/Channel.hpp"

/*
RPL_INVITING (341)
ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442)
ERR_CHANOPRIVSNEEDED (482)
ERR_USERONCHANNEL (443)
*/

bool	Server::isInviteCmdValid(Channel* channel, std::vector<string>& cmd, int fd)
{
	if (!channel->isOperator(getClient(fd)->getUsername()))
	{
		sendMsg(ERR_CHANOPRIVSNEEDED(getClient(fd)->getUsername(), cmd[2]), fd);
		return (false);
	}
	if (!channel)
	{
		sendMsg(ERR_NOSUCHCHANNEL(getClient(fd)->getUsername(), cmd[2]), fd);
		return (false);
	}
	if (cmd.size() < 3)
	{
		sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getUsername(), cmd[0]), fd);
		return(false);
	}
	return (true);
}

bool	Server::isPositif(string cmd)
{
	bool flag = true;
	
	if (cmd[0] == '-')
		flag = false;
	return (flag);
}

std::vector<string>	Server::divisor(string cmd, bool flag) 
{
	size_t	i;
	size_t	init = 0;
	std::vector<string>	vec;
	int		num_ch = 0;
	if (flag)
		i = 0;
	else
		i = 1;
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
	(void) fd;
	std::vector<Client>&	lstClients = getClients();
	
	if (lstClients.size() <= 0)
		return (false);
	for (size_t i = 0; i < lstClients.size(); i++)
	{
		if (lstClients[i].getNickname() == nickName)
			return (true);
	}
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

void	Server::userOnChannel(Channel* channel, std::vector<string> nickName, string command, int fd)
{
	for (size_t i = 0; i < nickName.size(); i++)
	{
		if (channel->hasClient(nickName[i]))
			sendMsg(ERR_USERONCHANNEL(getClient(fd)->getNickname(), nickName[i], command), fd);
	}
	return ;
}

void	Server::invitationManagement(Channel* channel, std::vector<string>& nickName, int fd, bool flag)
{	
	(void) fd;
	for (size_t i = 0; i < nickName.size(); i++)
	{
		if (flag)
			channel->addInvitation(nickName[i]);
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
	
	if (cmd.size() < 2)
		return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), command), fd));
	
	Channel*	channel = findChannel(cmd[2]);
	if (!isInviteCmdValid(channel, cmd, fd))
		return ;

	bool flag = isPositif(cmd[1]);
	std::vector<string> nickName = divisor(cmd[1], flag);
	if (!nicknameExist(nickName, fd))
		return ;
	userOnChannel(channel, nickName, command, fd);
	invitationManagement(channel, nickName, fd, flag);
}
