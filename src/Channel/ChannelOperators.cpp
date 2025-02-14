#include "../../include/Channel.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

// Return the username of the operators
const std::vector<string> &Channel::getOperators() const {
	return (_operators);
}

// Add the operators to _operators
void Channel::addOperator(string clientname) {
	std::cout << clientname << " addOperator" << std::endl;
	std::vector<string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); ++it){
		if (*it == clientname) {
			std::cout << "Client [" << clientname << "] already operator" << std::endl;
			return ;
		}
	}
	_operators.push_back(clientname);
	std::cout << "Client [" << clientname << "] now is operator" << std::endl;
	// We can use the return value if the client is allready
	// an operator. Should we ?
}

// Remove operators from _operatos
void Channel::removeOperator(string clientname) {
	std::vector<string>::iterator it = std::find(_operators.begin(), _operators.end(), clientname);

	if (it != _operators.end()) {
		_operators.erase(it);
		std::cout << "Client [" << clientname << "] not an operator anymore" << std::endl;
	}
	std::cout << "Client [" << clientname << "] not an operator" << std::endl;
	// Maybe we need to implement a different message error
}

// Checks if the clientname is ins _operators
bool Channel::isOperator(string clientname) const {
	std::vector<string>::const_iterator it = std::find(_operators.begin(), _operators.end(), clientname);

	//for (it == _operators.begin(); it != _operators.end(); ++it) {
		//if (*it == clientname) {
			//	return true;
			//}
			//}
	if (it != _operators.end()) {
		std::cout << "Client [" << clientname << "] is operator" << std::endl;
		return true;
	}
	std::cout << "Client [" << clientname << "] not an operator" << std::endl;
	return false;
}

// Checks if the mode given is 1(set) or 0(unset)
bool	Channel::isModeSet(Mode mode) { 
	std::cout << "Checking mode: " << mode << ", current modes: " << this->_modes << std::endl;
	return _modes & mode; }

// Set the mode given changing the 0 to 1
void	Channel::setMode(Mode mode) { _modes |= mode; }

// Unsets the mode given changing the 1 to 0
void	Channel::unsetMode(Mode mode) { _modes &= ~mode; }
