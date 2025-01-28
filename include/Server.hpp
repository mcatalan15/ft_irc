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
		std::vector<Client>			_clients; // Poner puntero?????????!!!!!!!
		std::vector<struct pollfd>	_pollFds; //buffer. Structura para manejar mas de 1 fd(cliente)
		struct sockaddr_in6			_address;
		string						_password;
		// For map (switch case)
		static std::map<std::string, void (Server::*)(std::vector<string>&, int)> createCmdMap();
	    static const std::map<std::string, void (Server::*)(std::vector<string>&, int)> cmdMap;

	public:
		Server(int port, string password);
		~Server();

		static void			signalHandler(int signum);
		void			client_process();
		void			client_exist(int fd);
		void			new_client(int &numfd);
		void			removeClient(int fd);
		void			removeFd(int fd);
		void			msgManagement(int fd);
		bool			isRegistered(int fd);
		void			sendMsg(string msg, int fd);

		//Fds management
		void			closeFds();
		
		
		//Getters
		string		getPassword();
		Client			*getClient(int fd);
		Client			*getClientNickname(std::string nickname);
		
		//isUsed commands
		bool		nickIsUsed(string cmd);
		bool		userIsUsed(string cmd);

		//Commands
		void		passCmd(std::vector<string>& cmd, int fd);
		void		nickCmd(std::vector<string>& cmd, int fd);
		void		userCmd(std::vector<string>& cmd, int fd);
		void		quitCmd(std::vector<string>& cmd, int fd);
		void		modeCmd(std::vector<string>& cmd, int fd);
		void		joinCmd(std::vector<string>& cmd, int fd);
		void		partCmd(std::vector<string>& cmd, int fd);
		void		topicCmd(std::vector<string>& cmd, int fd);
		void		kickCmd(std::vector<string>& cmd, int fd);
		void		privmsgCmd(std::vector<string>& cmd, int fd);
		void		inviteCmd(std::vector<string>& cmd, int fd);
		void		whoisCmd(std::vector<string>& cmd, int fd);
		void		adminCmd(std::vector<string>& cmd, int fd);
		void		infoCmd(std::vector<string>& cmd, int fd);
		void		pongCmd(std::vector<string>& cmd, int fd);
		void		pingCmd(std::vector<string>& cmd, int fd);
		void		capCmd(std::vector<string>& cmd, int fd);
};

#endif
