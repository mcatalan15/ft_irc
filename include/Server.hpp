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
		// For map (switch case)
		static std::map<std::string, void (Server::*)(std::string&, int)> createCmdMap();
	    static const std::map<std::string, void (Server::*)(std::string&, int)> cmdMap;

	public:
		Server(int port, string password);
		~Server();

		static void			signalHandler(int signum);
		void			client_process();
		void			client_exist(int fd);
		void			new_client(int &numfd);
		void			removeClient(int fd);
		void			removeFd(int fd);
		void			msgManagement(string &command, int fd);
		bool			isRegistered(int fd);
		void			sendMsg(string msg, int fd);

		//Fds management
		void			closeFds();
		
		
		//Getters
		string		getPassword();
		Client			*getClient(int fd);
		Client			*getClientNickname(std::string nickname);
		
		//Commands
		void		passCmd(string &cmd, int fd);
		void		nickCmd(string &cmd, int fd);
		void		userCmd(string &cmd, int fd);
		void		quitCmd(string &cmd, int fd);
		void		modeCmd(string &cmd, int fd);
		void		joinCmd(string &cmd, int fd);
		void		partCmd(string &cmd, int fd);
		void		topicCmd(string &cmd, int fd);
		void		kickCmd(string &cmd, int fd);
		void		privmsgCmd(string &cmd, int fd);
		void		inviteCmd(string &cmd, int fd);
		void		whoisCmd(string &cmd, int fd);
		void		adminCmd(string &cmd, int fd);
		void		infoCmd(string &cmd, int fd);
		void		pongCmd(string &cmd, int fd);
		void		pingCmd(string &cmd, int fd);
};

#endif
