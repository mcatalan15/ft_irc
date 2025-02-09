#include "../../include/Channel.hpp"

#include <iostream>
#include <stdexcept>

const std::set<Client*> &Channel::getOperators() const
{
	return (_operators);
}

void Channel::addOperator(Client *client)
{
	_operators.insert(client);

	// We can use the return value if the client is allready
	// an operator. Should we ?
}

void Channel::removeOperator(Client *client)
{
	std::set<Client*>::iterator it = getOperators().find(client);
	if (it == getOperators().end())
		throw std::runtime_error("Client is not in the invitation list.");
	// Maybe we need to implement a different message error
	_operators.erase(client);
}

bool Channel::isOperator(Client *client) const
{
	std::set<Client*>::iterator it = getOperators().find(client);
	if (it == getOperators().end())
		return (false);
	return (true);
}

bool	Channel::isModeSet(Mode mode) { 
	std::cout << "Checking mode: " << mode << ", current modes: " << this->_modes << std::endl;
	return _modes & mode; }


void	Channel::setMode(Mode mode) { _modes |= mode; }

void	Channel::unsetMode(Mode mode) { _modes &= ~mode; }

