#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "irc.hpp"
# include "errorLog.hpp"

class IRCSocket {
	private:
		int server_fd;
		struct sockaddr_in server_addr; // Server address struct

	public:

};

#endif
