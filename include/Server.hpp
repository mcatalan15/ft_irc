#ifndef SERVER_HPP
# define SERVER_HPP
# include "Irc.hpp"

class Server {
	private:
        int                     fd;
        struct sockaddr_in      address;
        std::map<int, Client>   clients_connected;

	public:
};
#endif