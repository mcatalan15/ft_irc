#include "../../include/Channel.hpp"

#include <stdexcept>

const std::set<Client*> &Channel::getBannedClients() const
{
    return (_bannedClients);
}

const std::set<Client*> &Channel::getInvitedClients() const
{
    return (_invitedClients);
}

void Channel::addBannedClient(Client *client)
{
    _bannedClients.insert(client);
    // We can use the return value if the client is allready
    // banned. Should we ?    
}

void Channel::addInvitation(Client *client)
{
    _invitedClients.insert(client);
    // We can use the return value if the client is allready
    // invited. Should we ?
}

void Channel::removeInvitation(Client *client)
{
	std::set<Client*>::iterator it = getInvitedClients().find(client);
    if (it == getInvitedClients().end())
        throw std::runtime_error("Client is not in the invitation list.");
    // Maybe we need to implement a different message error
    _invitedClients.erase(client);
}

bool Channel::isInvited(Client *client) const
{
    std::set<Client*>::iterator it = getInvitedClients().find(client);
    if (it == getInvitedClients().end())
        return (false);
    return (true);
}

bool Channel::isBanned(Client *client) const
{
    std::set<Client*>::iterator it = getBannedClients().find(client);
    if (it == getBannedClients().end())
        return (false);
    return (true);
}
