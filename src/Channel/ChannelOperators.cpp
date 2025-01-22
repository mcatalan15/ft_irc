#include "../../include/Channel.hpp"

#include <stdexcept>

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
