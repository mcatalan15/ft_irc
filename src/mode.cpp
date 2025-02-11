#include "../include/Server.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <ostream>
#include <cctype>
#include <set>
#include <vector>

bool	Server::validFlags(string cmd)
{
	std::vector<bool> charactersRepeat(128, false);

	for (std::string::iterator it = cmd.begin(); it != cmd.end(); it++)
	{
		char c = *it;
		if (charactersRepeat[static_cast<unsigned char>(c)])
		{
			// LANNZAR MENSAJE DE ERROR
            return (false);
		}
        charactersRepeat[static_cast<unsigned char>(c)] = true;
    }
    return (true);
}

bool	Server::isFlagMode(Channel* channel, std::vector<string>& cmd, int num)
{
	(void)channel;
	if (cmd[2].size() < 2) {
		std::cout << "Invalid flag size" << std::endl;
		return (false);
	}
	if (num == 1)
	{
		for (size_t i = 1; i < cmd[2].size(); i++)
		{
			if (cmd[2].find_first_of(CHANNEL_MODES) == string::npos)
				return (false);
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
	if (!isFlagMode(channel, cmd, 2))
	{
		if (!isFlagMode(channel, cmd, 1))
		{
			sendMsg("no such modes...\n", fd);
			return (false);
		}
		return (true);
	}
	// verificar si los comandos son validos
	return (true);
}

bool	Server::isModeCmdValid(Channel* channel, std::vector<string>& cmd, int fd)
{
	if (!channel)
	{
		sendMsg(ERR_NOSUCHCHANNEL(getClient(fd)->getNickname(), cmd[1]), fd);
		return (false);
	}
	if (cmd.size() < 3)
	{
		sendMsg(RPL_CREATIONTIME(getClient(fd)->getNickname(), cmd[1], getCreationTime()), fd);
		return(false);
	}
	if (!channel->isOperator(getClient(fd)->getUsername())) {
		std::vector<string> list = channel->getOperators();
		for (std::vector<string>::iterator it = list.begin(); it != list.end(); ++it) {
        	std::cout << "OperList: " << *it << std::endl;
        }
		std::cout << "noOper " << getClient(fd)->getNickname() << std::endl;
		sendMsg("ERROR NO ES OPERATOR buscar err\n", fd);
		return (false);
	}
	if (cmd[2][0] != '+' && cmd[2][0] != '-'){

		sendMsg("no operator(+-) found\n", fd);
		return (false);
	}
	// Comprobar que no hayan flags  repetidas
	return (true);
}

Client*	Server::findNickname(string nick, Channel* channel)
{
	const std::vector<string>&	lstClients = channel->getClients();
	std::cout << "findNickname: " << std::endl;
	(void)nick;
	(void)channel;
	//(void)lstClients;
	//std::cout << "sizeclients: " << channel->getClients().size();
	if (lstClients.size() <= 0) {
		std::cout << "No hay clients en este channel" << std::endl;
		return NULL;
	}
	for (size_t i = 0; i < lstClients.size(); i++)
	{
		if (getUser(lstClients[i])->getNickname() == nick)
			return getUser(lstClients[i]);
	}
	/*for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickname() == nick)
			return &_clients[i];
	}*/
	return NULL;
}

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

void	Server::flagModeK(bool flag, Channel* channel, string cmd)
{
	if (flag)
	{
		channel->setPassword(cmd);
		channel->setMode(PASSWORD_SET);
	}
	else
		channel->unsetMode(PASSWORD_SET);
}

void	Server::flagModeO(bool flag, Channel* channel, string cmd)
{
	if (flag)
	{
		std::cout << "entra a addOperator\n";
		channel->addOperator(findNickname(cmd, channel)->getUsername());
	 	std::cout << "para confirmar\n" << std::endl;//REVISAR que cliente es
	}
	else
	{
		std::cout << "entra a removeOperator\n";
		channel->removeOperator(findNickname(cmd, channel)->getUsername());
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
	(void)fd;
	bool flag = false;
	if (cmd[2][0] == '+')
		flag = true;
	size_t j = 3;
	for (size_t i = 1; i < cmd[2].size(); i++)
	{
		if (cmd[2][i] == 'i')
			flagModeI(flag, channel);
		if (cmd[2][i] == 't') {
			flagModeT(flag, channel);
		}
		if (cmd[2][i] == 'o') {
			if (cmd.size() >= (j + 1)) {
				//check if the argument is valid
				flagModeO(flag, channel, cmd[j]);
				j++;
			}
		}
		if (cmd[2][i] == 'k') {
			if (flag)
			{
				if (cmd.size() >= (j + 1))
				{
					flagModeK(flag, channel, cmd[j]);
					j++;
				}
			//	else
					//mensaje de error
			}
			else
				flagModeK(flag, channel, cmd[j]);
		}
		if (cmd[2][i] == 'l') {
			if (flag)
			{
				if (cmd.size() >= (j + 1))
				{
					if (!isNumber(cmd[j]))
					// put error_message;
						return ;
					flagModeL(flag, channel, cmd[j]);
					j++;
				}
				else {
					//soltar mensaje de error
				}
			}
			else
				flagModeL(flag, channel, cmd[j]);
		}
	}
	std::cout << "Sale de Modemanagement" << std::endl;
}

void	Server::modeCmd(std::vector<string>& cmd, int fd)
{
	std::cout << "MODE cmd" << std::endl;
	printVecStr(cmd);
	if (cmd.size() < 2)
		return (sendMsg("no channel as arg ERR_NEEDMOREPARAMS (461) \n", fd));
	Channel*	channel = findChannel(cmd[1]);
	if (!isModeCmdValid(channel, cmd, fd))
		return ;
	if (!checkModeFlags(channel, cmd, fd))
		return ;
	if (!validFlags(cmd[2]))
		return ;
	modeManagement(channel, cmd, fd);
}
