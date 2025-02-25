#include "../../include/Channel.hpp"

const std::vector<string> &Channel::getClients() const
{
	return (_clients);
}

void Channel::addClient(string clientname)
{
	if (isFull())
		std::cerr << "Channel is full" << std::endl;
	else
		_clients.push_back(clientname);
		//throw std::runtime_error("Channel is full");
		// Need to implement ErrorMessage with : throw
}

void Channel::removeClient(string clientname)
{
	std::vector<string>::iterator it = std::find(_clients.begin(), _clients.end(), clientname);
	if (it == _clients.end())
		std::cerr << "Client doesn't exist" << std::endl;
	else
		_clients.erase(it);
	//throw std::runtime_error("Client doesn't exist");
	// maybe We need to put a diff message
}

bool Channel::hasClient(string clientname) const
{
	std::vector<string>::const_iterator it = std::find(_clients.begin(), _clients.end(), clientname);
	std::cout << "llega\n";
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
