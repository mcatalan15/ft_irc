#include "../include/Irc.hpp"

Client::Client()
{
}

struct sockaddr	Client::getAddr()
{
	return (this->address);
}
