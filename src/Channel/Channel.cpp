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

Channel::~Channel()
{
    //NEED TO IMPLEMENT
}
