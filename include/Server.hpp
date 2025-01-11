#ifndef SERVER_HPP
# define SERVER_HPP
# include "Irc.hpp"

using std::string;

class Client;

class Server {
	private:
		int                     fd;
		int			opt;
		std::map<int, Client>   clients_connected;
		struct sockaddr_in6     address;
		string			password;

	public:
		Server(int port, string password);

		void	client_process();
};
#endif
