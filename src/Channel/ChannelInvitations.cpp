#include "../../include/Channel.hpp"

#include <stdexcept>
#include <string>
#include <vector>

const std::vector<string> &Channel::getBannedClients() const { return (_bannedClients); }

const std::vector<string> &Channel::getInvitedClients() const { return (_invitedClients); }

// Checks if banned and if not adds the client to banned list
void	Channel::addBannedClient(string clientname) {
	std::vector<string>::iterator it;
		
	for (it = _bannedClients.begin(); it != _bannedClients.end(); ++it) {
		if (*it == clientname) {
			std::cout << "Client [" << clientname << "] already banned" << std::endl;
			return ; //Client already banned, exit
		}
	}
	_bannedClients.push_back(clientname);
	std::cout << "Client [" << clientname << "] banned" << std::endl;
}

// Checks if invited. If not adds the client to the invitation list
void Channel::addInvitation(string clientname) {
	std::vector<string>::iterator it;
		
	for (it = _invitedClients.begin(); it != _invitedClients.end(); ++it) {
		if (*it == clientname) {
			std::cout << "Client [" << clientname << "] already invited" << std::endl;
			return ; //Already invited
		}
	}
	_invitedClients.push_back(clientname);
	std::cout << "Client [" << clientname << "] invited" << std::endl;
	// We can use the return value if the client is allready
	// invited. Should we ?
}

void Channel::removeInvitation(string clientname) {
	std::vector<string>::iterator it;

	for (it = _invitedClients.begin(); it != _invitedClients.end(); ++it) {
		if (*it == clientname) {
			_invitedClients.erase(it);
			std::cout << "Client [" << clientname << "] invitation removed" << std::endl;
			return;
		}
	}
	// Maybe we need to implement a different message error
	std::cout << "Client [" << clientname << "] is not in the invitation list." << std::endl;
}

bool Channel::isInvited(string clientname) const {
	std::vector<string>::const_iterator it;

	for (it = _invitedClients.begin(); it != _invitedClients.end(); ++it) {
		if (*it == clientname) {
			std::cout << "Client [" << clientname << "] not invited" << std::endl;
			return true;
		}
	}
	std::cout << "Client [" << clientname << "] not invited" << std::endl;
	return false;
}

bool Channel::isBanned(string clientname) const
{
	std::vector<string>::const_iterator it;
		
	for (it = _bannedClients.begin(); it != _bannedClients.end(); it++) {
		if (*it == clientname) {
			std::cout << "Client [" << clientname << "] is banned" << std::endl;
			return true;
		}
	}
	std::cout << "Client [" << clientname << "] is not banned" << std::endl;
	return false;
}
