#include "../../include/Channel.hpp"

Channel::Channel(const std::string& name) : _modes(0)
{
    _name = name;
    _topic.clear();
    _clients.clear();
    _userLimit = MAX_CLIENTS;
    _operators.clear();
    _invitedClients.clear();
    _password.clear();
    setCreationTime();
}

Channel::Channel(const Channel& cpy)
{
 	_name = cpy._name;
    _clients = cpy._clients;
    _userLimit = cpy._userLimit;
    _operators = cpy._operators;
    _invitedClients = cpy._invitedClients;
    _password = cpy._password;
    _modes = cpy._modes;
}

Channel::~Channel()
{
    //NEED TO IMPLEMENT
}
