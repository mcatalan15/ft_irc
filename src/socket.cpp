# include "irc.hpp"

// Default Constructor
IRCSocket::IRCSocket() : server_fd(-1)
{
	memset(&server_addr, 0, sizeof(server_addr));
}

// Parameterized Constructor
IRCSocket::IRCSocket(int port, IRCErrorLog *error) : server_fd(-1)
{
	// Socket setup code here (if necessary for initialization)
}

// Copy Constructor
IRCSocket::IRCSocket(const IRCSocket &other)
{
	// Copy necessary fields (if applicable)
	server_fd = other.server_fd;
	server_addr = other.server_addr;
}

// Copy Assignment Operator
IRCSocket &IRCSocket::operator=(const IRCSocket &other)
{
	if (this != &other)
	{
		// Clean up existing resources, if any
		if (server_fd >= 0)
		{
			close(server_fd);
		}

		// Copy necessary fields
		server_fd = other.server_fd;
		server_addr = other.server_addr;
	}
	return *this;
}

// Destructor
IRCSocket::~IRCSocket()
{
	if (server_fd >= 0)
	{
		close(server_fd); // Close the socket if open
	}
}
