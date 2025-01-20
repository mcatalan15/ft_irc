#ifndef SERVER_HPP
# define SERVER_HPP
# include "Client.hpp"
#include <sys/poll.h>

using std::string;

class Client;

class Server {
	private:
		int                     fd;
		int			opt;
		std::map<int, Client>   clients_connected;
		struct pollfd         fds[1024]; //buffer. Structura para manejar mas de 1 fd(cliente)
		struct sockaddr_in6     address;
		string			password;

	public:
		Server(int port, string password);

		void      client_process();
		void      client_exist(int &numfd, int i);
		void      new_client(int &numfd, int i);
};
#endif
