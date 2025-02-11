#include "../../include/Channel.hpp"

#include <stdexcept>
#include <string>
#include <vector>

const std::vector<string> &Channel::getBannedClients() const { return (_bannedClients); }

const std::vector<string> &Channel::getInvitedClients() const { return (_invitedClients); }

void	Channel::addBannedClient(string clientname)
{
	std::vector<string>::iterator it;
		
	for (it = _bannedClients.begin(); it = _bannedClients.end(); it++)
	{
		if (*it == clientname)
			break ;
	}
	_bannedClients.insert(it);
	// We can use the return value if the client is allready
	// banned. Should we ?    
}

void Channel::addInvitation(string clientname)
{
	std::vector<string>::iterator it;
		
	for (it = _invitedClients.begin(); it = _invitedClients.end(); it++)
	{
		if (*it == clientname)
			break ;
	}
	_invitedClients.insert(it);
	// We can use the return value if the client is allready
	// invited. Should we ?
}

void Channel::removeInvitation(string clientname)
{
	std::vector<string>::iterator it;
	
	for (it = _invitedClients.begin(); it = _invitedClients.end(); it++)
	{
		if (*it == clientname)
			break ;
	}
	if (it == getInvitedClients().end())
		throw std::runtime_error("Client is not in the invitation list.");
	// Maybe we need to implement a different message error
	_invitedClients.erase(it);
}

bool Channel::isInvited(string clientname) const
{
	std::vector<string>::iterator it;
		
	for (it = _invitedClients.begin(); it = _invitedClients.end(); it++)
	{
		if (*it == clientname)
			break ;
	}
	if (it == getInvitedClients().end())
		return (false);
	return (true);
}

bool Channel::isBanned(string clientname) const
{
	std::vector<string>::iterator it;
		
	for (it = _bannedClients.begin(); it = _bannedClients.end(); it++)
	{
		if (*it == clientname)
			break ;
	}
	if (it == getBannedClients().end())
		return (false);
	return (true);
}
