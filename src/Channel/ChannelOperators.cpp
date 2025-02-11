#include "../../include/Channel.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

const std::vector<string> &Channel::getOperators() const
{
	return (_operators);
}

void Channel::addOperator(string clientname)
{
	//std::cout << client << " addOperator" << std::endl;
	_operators.insert(std::find(_operators.begin(), _operators.end(), clientname));
	std::cout << "sddgadfgdfgafgadfhdghhlilgusdfgluifdlgi wruilg hlw" << std::endl;
	// We can use the return value if the client is allready
	// an operator. Should we ?
}

void Channel::removeOperator(string clientname)
{
	std::set<string>::iterator it = std::find(_operators.begin(), _operators.end(), clientname);
	if (it == getOperators().end())
		throw std::runtime_error("Client is not in the invitation list.");
	// Maybe we need to implement a different message error
	_operators.erase(it);
}

bool Channel::isOperator(string clientname) const
{
	std::set<string>::iterator it = std::find(_operators.begin(), _operators.end(), clientname);
	if (it == getOperators().end())
		return (false);
	return (true);
}

bool	Channel::isModeSet(Mode mode) { 
	std::cout << "Checking mode: " << mode << ", current modes: " << this->_modes << std::endl;
	return _modes & mode; }


void	Channel::setMode(Mode mode) { _modes |= mode; }

void	Channel::unsetMode(Mode mode) { _modes &= ~mode; }

