#ifndef SERVER_HPP
#define SERVER_HPP
#include "Client.hpp"
#include <sys/poll.h>
#include <vector>

using std::string;

class Client;

class Server {
	private:
		int							_fd;
		int							_opt;
//		int							_port;
		static bool					_signal;
		std::vector<Client>			_clients_connected;
		std::vector<struct pollfd>	_fds; //buffer. Structura para manejar mas de 1 fd(cliente)
		struct sockaddr_in6			_address;
		string						_password;

	public:
		Server(int port, string password);
		~Server();

		static void			signalHandler(int signum);
		void			client_process();
		void			client_exist(int &numfd, int i);
		void			new_client(int &numfd);
};

#endif
