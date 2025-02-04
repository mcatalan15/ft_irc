#include "../../include/Channel.hpp"

Channel::Channel(const std::string& name)
{
    _name = name;
    _clients.clear();
    _userLimit = 100;
    _operators.clear();
    _invitedClients.clear();
    _password.clear();
}

Channel::Channel(const Channel& cpy)
{
 	_name = cpy._name;
    _clients = cpy._clients;
    _userLimit = cpy._userLimit;
    _operators = cpy._operators;
    _invitedClients = cpy._invitedClients;
    _password = cpy._password;
}

Channel::~Channel()
{
    //NEED TO IMPLEMENT
}
