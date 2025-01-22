#include "../../include/Channel.hpp"

#include <stdexcept>
#include <algorithm>

// Client management
const std::vector<Client*> &Channel::getClients() const
{
    return (_clients);
}

void Channel::addClient(Client *client)
{
    if (isFull())
        throw std::runtime_error("Channel is full");
    // Need to implement ErrorMessage with : throw
    _clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
    auto it = std::find(_clients.begin(), _clients.end(), client);
    if (it == _clients.end())
        throw std::runtime_error("Client doesn't exist");
    // maybe We need to put a diff message
    _clients.erase(it);
}

bool Channel::hasClient(Client *client) const
{
    auto it = std::find(_clients.begin(), _clients.end(), client);
    if (it == _clients.end())
        return (false);
    return (true);
}

// Operator management
const std::set<Client*> &Channel::getOperators() const
{
    return (_operators);
}

void Channel::addOperator(Client *client)
{
    _operators.insert(client);
    // We can use the return value if the client is allready
    // an operator. Should we ?
}

void Channel::removeOperator(Client *client)
{
    auto it = getOperators().find(client);
    if (it == getOperators().end());
        throw std::runtime_error("Client is not in the invitation list.");
    // Maybe we need to implement a different message error
    _operators.erase(client);
}

bool Channel::isOperator(Client *client) const
{
    auto it = getOperators().find(client);
    if (it == getOperators().end())
        return (false);
    return (true);
}
