#include "../../include/Channel.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>

const std::vector<string> &Channel::getClients() const
{
	return (_clients);
}

void Channel::addClient(string clientname) {
	if (isFull())
		throw std::runtime_error("Channel is full");
		// Need to implement ErrorMessage with : throw
	_clients.push_back(clientname);
}

void Channel::removeClient(string clientname)
{
	std::vector<string>::iterator it = std::find(_clients.begin(), _clients.end(), clientname);
	if (it == _clients.end())
		throw std::runtime_error("Client doesn't exist");
	// maybe We need to put a diff message
	_clients.erase(it);
}

bool Channel::hasClient(string clientname) const
{
	std::vector<string>::const_iterator it = std::find(_clients.begin(), _clients.end(), clientname);
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
