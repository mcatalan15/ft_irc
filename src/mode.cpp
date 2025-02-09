#include "../include/Server.hpp"
#include <iostream>
#include <ostream>
/*
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
		// Y este MSG?
		sendMsg(RPL_CREATIONTIME(getClient(fd)->getNickname(), cmd[1], getCreationTime()), fd);
		return(false);
	}
	if (!channel->isOperator(getClient(fd)))
	{
		sendMsg("buscar que hay que enviar\n", fd);
		return (false);
	}
	// Comprobar que no haya repetidas
	return (true);
}

Client*	Server::findNickname(string nick, Channel* channel)
{
	const std::vector<Client*>&	lstClients = channel->getClients();
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
		if (lstClients[i]->getNickname() == nick)
			return lstClients[i];
	}
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickname() == nick)
			return &_clients[i];
	}
	return NULL;
}

void	Server::flagModeL(bool flag, Channel* channel, string cmd)
{
	if (flag) 
	{
		std::stringstream ss(cmd);
		int num;
		ss >> num;
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
		channel->addOperator(findNickname(cmd, channel));
	 	std::cout << "para confirmar\n" << std::endl;//REVISAR que cliente es
	}
	else
	{
		std::cout << "entra a removeOperator\n";
		channel->removeOperator(findNickname(cmd, channel));
	}
}

void	Server::flagModeT(bool flag)
{
	if (flag)
		channel->setMode(TOPIC_RESTRICTED);
	else
		channel->unsetMode(TOPIC_RESTRICTED);
}

void	Server::flagModeI(bool flag)
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
	if (cmd[2][0] != '+' && cmd[2][0] != '-')
		return (sendMsg("no operator(+-) found\n", fd));
	if (cmd[2][0] == '+')
		flag = true;
	size_t j = 3;
	for (size_t i = 1; i < cmd[2].size(); i++)
	{
		if (cmd[2][i] == 'i')
			flagModeI(flag);
		if (cmd[2][i] == 't') {
			flagModeT(flag);
		}
		if (cmd[2][i] == 'o') {
			if (cmd.size() >= (j + 1)) {
				flagModeO(flag, channel, cmd[j]);
				j++;
			}
		}
		if (cmd[2][i] == 'k') {
			if (flag) 
			{
				if (cmd.size() >= (j + 1))
				{
					flagModeK(flag, channel, cmd, fd, j);
					j++;
				}				
				else
					//mensaje de error
			}
			else
				flagModeK(flag, channel, cmd, fd, j);
		}
		if (cmd[2][i] == 'l') {
			if (flag) {
				if (cmd.size() >= (j + 1))
				{
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
*/
void	Server::modeCmd(std::vector<string>& cmd, int fd)
{
	std::cout << "MODE cmd" << std::endl;
	printVecStr(cmd);
	(void)fd;
	/*if (cmd.size() < 2)
		return (sendMsg("no channel as arg ERR_NEEDMOREPARAMS (461) \n", fd));
	Channel*	channel = findChannel(cmd[1]);
	if (!isModeCmdValid(channel, cmd, fd))
		return ;
	if (!checkModeFlags(channel, cmd, fd))
		return ;
	// necesitamos checker si los modes son del mismo typo
	modeManagement(channel, cmd, fd);*/
}
