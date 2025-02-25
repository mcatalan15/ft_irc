#include "../../include/Channel.hpp"

//#include <stdexcept>

const std::vector<string> &Channel::getBannedClients() const { return (_bannedClients); }

const std::vector<string> &Channel::getInvitedClients() const { return (_invitedClients); }

// Checks if banned and if not adds the client to banned list
void	Channel::addBannedClient(string clientname)
{
	std::vector<string>::iterator it;
	for (it = _bannedClients.begin(); it != _bannedClients.end(); ++it)
	{
		if (*it == clientname)
			return ; //Client already banned, exit
	}
	_bannedClients.push_back(clientname);
}

// Checks if invited. If not adds the client to the invitation list
void Channel::addInvitation(string clientname)
{
	std::vector<string>::iterator it;
	for (it = _invitedClients.begin(); it != _invitedClients.end(); ++it)
	{
		if (*it == clientname)
			return ; //Already invited
	}
	_invitedClients.push_back(clientname);
}

void Channel::removeInvitation(string clientname)
{
	std::vector<string>::iterator it;

	for (it = _invitedClients.begin(); it != _invitedClients.end(); ++it)
	{
		if (*it == clientname) {
			_invitedClients.erase(it);
			return;
		}
	}
}

bool Channel::isInvited(string clientname) const
{
	std::vector<string>::const_iterator it;

	for (it = _invitedClients.begin(); it != _invitedClients.end(); ++it)
	{
		std::cout << "invited: " << *it << std::endl;
		std::cout << "clientname: " << clientname << std::endl;
		if (*it == clientname)
			return true;
	}
	return false;
}

bool Channel::isBanned(string clientname) const
{
	std::vector<string>::const_iterator it;

	for (it = _bannedClients.begin(); it != _bannedClients.end(); it++)
	{
		if (*it == clientname)
			return true;
	}
	return false;
}
