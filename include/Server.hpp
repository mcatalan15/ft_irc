#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"

class Client;

class Server {
	private:
		int							_serverFd;
		int							_opt;
//		int							_port;
		static bool					_signal;
		std::vector<Client>			_clients;
		std::vector<struct pollfd>	_pollFds; //buffer. Structura para manejar mas de 1 fd(cliente)
		struct sockaddr_in6			_address;
		string						_password;

	public:
		Server(int port, string password);
		~Server();

		static void			signalHandler(int signum);
		void			client_process();
		void			client_exist(int fd);
		void			new_client(int &numfd);
		void			removeClient(int fd);
		void			removeFd(int fd);
		
		//Fds management
		void			closeFds();
		
		
		//Getters
		string		getPassword();
		Client			*getClient(int fd);
		Client			*getClientNickname(std::string nickname);
};

#endif
