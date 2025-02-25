#include "../../include/Channel.hpp"

// Return the username of the operators
const std::vector<string> &Channel::getOperators() const { return (_operators); }

// Add the operators to _operators
void Channel::addOperator(string clientname)
{
	std::vector<string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); ++it){
		if (*it == clientname)
			return ;
	}
	_operators.push_back(clientname);
}

// Remove operators from _operatos
void Channel::removeOperator(string clientname) 
{
	std::vector<string>::iterator it = std::find(_operators.begin(), _operators.end(), clientname);

	if (it != _operators.end())
		_operators.erase(it);
}

// Checks if the clientname is ins _operators
bool Channel::isOperator(string clientname) const
{
	std::vector<string>::const_iterator it = std::find(_operators.begin(), _operators.end(), clientname);
	if (it != _operators.end())
		return true;
	return false;
}

// Checks if the mode given is 1(set) or 0(unset)
bool	Channel::isModeSet(Mode mode) { return _modes & mode; }

// Set the mode given changing the 0 to 1
void	Channel::setMode(Mode mode) { _modes |= mode; }

// Unsets the mode given changing the 1 to 0
void	Channel::unsetMode(Mode mode) { _modes &= ~mode; }
