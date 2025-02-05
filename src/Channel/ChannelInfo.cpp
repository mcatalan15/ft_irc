#include "../../include/Channel.hpp"

const string Channel:: getName() const { return (_name); }

// Password management
string Channel::getPassword() const { return (_password); }

void	Channel::setPassword(const string& password) { _password = password; }
