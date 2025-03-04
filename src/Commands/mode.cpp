#include "../../include/Server.hpp"

void	Server::sendModeGeneralMsg(Channel *channel, string param, string target, int fd)
{
	string msg = "MODE ";
	msg.append(channel->getName());
	msg.append(" ");
	msg.append(param);
	if (target != "NULL")
	{
		msg.append(" ");
		msg.append(target);
		msg.append(" ");
	}
	msg.append(CRLF);
	sendMsg(msg, fd);
	sendMsgToChannel(msg, channel, fd);
	return ;
}

bool	Server::validFlags(Channel* channel, string modeChar, int fd, bool sign)
{
	std::vector<bool> charactersRepeat(128, false);

	string str = "";
	if (sign == true)
		str.append("+");
	else
		str.append("-");
	for (std::string::iterator it = modeChar.begin(); it != modeChar.end(); it++)
	{
		char c = *it;
		if (charactersRepeat[static_cast<unsigned char>(c)])
		{
			str.append(modeChar);
			sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getNickname(), channel->getName(), str, c, " Mode Operator is repeat"), fd);
            return (false);
		}
        charactersRepeat[static_cast<unsigned char>(c)] = true;
    }
    return (true);
}

bool	Server::isFlagMode(char modeChar, int num, int fd)
{
	(void) num;
	string str = CHANNEL_MODES;
	
	if (str.find_first_of(modeChar) == string::npos)
	{
		sendMsg(ERR_UMODEUNKOWNFLAG(getClient(fd)->getUsername()), fd);
		return (false);
	}
	return true;
}

bool	Server::checkModeFlags(string modeChar, int fd)
{
	for (size_t i = 0; i < modeChar.size(); i++)
	{
		if (!isFlagMode(modeChar[i], 1, fd))
			return (false);	
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

bool Server::isNumber(string cmd)
{
    for (size_t i = 1; i <= cmd.size(); i++)
	{
		if (!isdigit(cmd[i]))
			return (false);
	}
	return (true);
}

void	Server::flagModeL(bool flag, Channel* channel, string param, int fd)
{
	if (!flag)
	{
		std::stringstream ss(param);
		int num;
		ss >> num;
		std::cout << num << std::endl;
		if (num > MAX_CLIENTS)
		{
			num = MAX_CLIENTS;
			sendModeGeneralMsg(channel, "+l", MAX_CLIENTS_STR, fd);
			channel->setUserLimit(num);
			channel->setMode(USER_LIMIT);
			return ;
		}
		if (num <= 0)
			return(sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), channel->getName(), "+l", param, "Is invalid"), fd));
		if (num < channel->getChannelSize())
			return(sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), channel->getName(), "+l", param, "Is too low"), fd));
		channel->setUserLimit(num);
		channel->setMode(USER_LIMIT);
		sendModeGeneralMsg(channel, "+l", param, fd);
	}
	else
	{
		channel->unsetMode(USER_LIMIT);
		channel->setUserLimit(MAX_CLIENTS);
		sendModeGeneralMsg(channel, "-l", "", fd);
	}
	return ;
}

void	Server::flagModeK(bool flag, Channel* channel, string param, int fd)
{
	if (!flag)
	{
		if (channel->hasPassword())
			return (sendMsg(ERR_KEYSET(channel->getName()), fd));
		channel->setPassword(param);
		channel->setMode(PASSWORD_SET);
		sendModeGeneralMsg(channel, "+k", "NULL", fd);
	}
	else
	{
		channel->setPassword("");
		channel->unsetMode(PASSWORD_SET);
		sendModeGeneralMsg(channel, "-k", "NULL", fd);
	}
	return ;
}

void	Server::flagModeO(bool flag, Channel* channel, string target, int fd)
{
	if (!flag)
	{
		if (channel->isOperator((findNickname(target, channel))->getUsername()))
			return(sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), channel->getName(), "+o", target, "Is allready an Operator"), fd)); 
		channel->addOperator(findNickname(target, channel)->getUsername());
		sendModeGeneralMsg(channel, "+o", target, fd);
	}
	else
	{
		if (!channel->isOperator(findNickname(target, channel)->getUsername()))
			return(sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), channel->getName(), "-o", target, "Is not an Operator"), fd)); 
		channel->removeOperator(findNickname(target, channel)->getUsername());
		sendModeGeneralMsg(channel, "-o", target, fd);
	}
	return ;
}

void	Server::flagModeT(bool flag, Channel *channel, int fd)
{
	if (!flag)
	{
		channel->setMode(TOPIC_RESTRICTED);
		sendModeGeneralMsg(channel, "+t", "NULL", fd);
	}
	else
	{
		channel->unsetMode(TOPIC_RESTRICTED);
		sendModeGeneralMsg(channel, "-t", "NULL", fd);
	}
	return ;
}

void	Server::flagModeI(bool flag, Channel *channel, int fd)
{
	if (!flag)
	{
		channel->setMode(INVITE_ONLY);
		sendModeGeneralMsg(channel, "+i", "", fd);
	}
	else
	{
		channel->unsetMode(INVITE_ONLY);
		sendModeGeneralMsg(channel, "-i", "", fd);
	}
	return ;
}

void	Server::modeManagement(Channel* channel, std::vector<string>& cmd, string modeChar, int fd)
{
	bool flag = (cmd[2][0] != '+' ? true : false);

	std::vector<string> param;
	if (cmd.size() > 3)
		param = divisor(cmd[3], true);
	size_t j = 0;
	for (size_t i = 0; i < modeChar.size(); i++)
	{
		if (modeChar[i] == 'i')
			flagModeI(flag, channel, fd);
		if (modeChar[i] == 't')
			flagModeT(flag, channel, fd);
		if (modeChar[i] == 'o')
		{
			if (param.size() > j)
			{
				Client* client = getNick(param[j]);
				if (client)
				{
					if (!channel->hasClient(client->getUsername()))
					{
						if (!flag)
							return(sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), channel->getName(), "+o", param[j], "Is not on that channel"), fd));
						else
							return(sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), channel->getName(), "-o", param[j], "Is not on that channel"), fd));
					}
				}
				else 
					return(sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), channel->getName(), "+o", param[j], "Is not on that channel"), fd));
				flagModeO(flag, channel, param[j++], fd);
			}
			else
			{
				if (!flag)
					return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getUsername(), "+o"), fd));
				else
					return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getUsername(), "-o"), fd));
			}
		}
		if (modeChar[i] == 'k')
		{
			if (!flag)
			{
				if (param.size() > j)
				{
					flagModeK(flag, channel, param[j], fd);
					j++;
				}
				else
					return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getUsername(), "+k"), fd));
			}
			else
				flagModeK(flag, channel, "NULL", fd);
		}
		if (modeChar[i] == 'l')
		{
			if (!flag)
			{
				if (param.size() > j)
				{
					if (isNumber(param[j]))
						return (sendMsg(ERR_INVALIDMODEPARAM(getClient(fd)->getUsername(), channel->getName(), "+l", param[j], "Invalid limit"), fd));
					flagModeL(flag, channel, param[j], fd);
					j++;
				}
				else
					return (sendMsg(ERR_NEEDMOREPARAMS(getClient(fd)->getUsername(), "+l"), fd));
			}
			else
				flagModeL(flag, channel, "NULL", fd);
		}
	}
	return ;
}
