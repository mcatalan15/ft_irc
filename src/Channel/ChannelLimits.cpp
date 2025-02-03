#include "../../include/Channel.hpp"

int Channel::getUserLimit() const
{
    return (_userLimit);
}

int Channel::setUserLimit(int userLimit)
{
    _userLimit = userLimit;
}

bool Channel::isFull() const
{
    if (_clients.size() >= _userLimit)
        return (true);
    return (false);
}
