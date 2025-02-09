#include "../../include/Channel.hpp"

#include <stdexcept>
#include <algorithm>

const std::vector<Client*> &Channel::getClients() const
{
	return (_clients);
}

void Channel::addClient(Client *client)
{
	if (isFull())
		throw std::runtime_error("Channel is full");
		// Need to implement ErrorMessage with : throw
	_clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
	std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it == _clients.end())
		throw std::runtime_error("Client doesn't exist");
	// maybe We need to put a diff message
	_clients.erase(it);
}

bool Channel::hasClient(Client *client) const
{
	std::vector<Client*>::const_iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it == _clients.end())
		return (false);
	return (true);
}
/*
Client*	Channel::findNickname(string nick)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getNickname() == nick)
			return _clients[i];	
	}
	return NULL;
	}*/
