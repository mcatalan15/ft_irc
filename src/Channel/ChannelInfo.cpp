#include "../../include/Channel.hpp"

const string Channel:: getName() const 
{
	std::cout << _name << std::endl;
	return (_name); 
}

// Password management
string Channel::getPassword() const { return (_password); }

void	Channel::setPassword(const string& password) { _password = password; }

bool	Channel::hasPassword() const
{
	return (_password.empty() ? false : true);
}
