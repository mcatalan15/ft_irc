#include "../../include/Channel.hpp"

#include <stdexcept>

const std::set<Client*> &Channel::getInvitedClients() const
{
    return (_invitedClients);
}

void Channel::addInvitation(Client *client)
{
    _invitedClients.insert(client);
    // We can use the return value if the client is allready
    // invited. Should we ?
}

void Channel::removeInvitation(Client *client)
{
    auto it = getInvitedClients().find(client);
    if (it == getInvitedClients().end());
        throw std::runtime_error("Client is not in the invitation list.");
    // Maybe we need to implement a different message error
    _invitedClients.erase(client);
}

bool Channel::isInvited(Client *client) const
{
    auto it = getInvitedClients().find(client);
    if (it == getOperators().end())
        return (false);
    return (true);
}
