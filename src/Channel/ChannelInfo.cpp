#include "../../include/Channel.hpp"

const string Channel::getName() const 
{
	return (_name); 
}

// Password management
string Channel::getPassword() const { return (_password); }

void	Channel::setPassword(const string& password) { _password = password; }

bool	Channel::hasPassword() const
{
	return (_password.empty() ? false : true);
}

const string Channel::getTopic() const { return _topic; }

void	Channel::setTopic(const string& topic) { _topic = topic; }
