#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <string>

class Client;
class Channel;

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
		std::vector<Channel>		_channels; //Sin puntero guardamos directamente object (no new)
		string						_creationTime;
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
		bool			isRegistered(int fd);

		//Fds management
		void			closeFds();
		void			remove_fd(int fd); // Remove save pollFds

		//Msg management
		void			msgManagement(int fd);
		bool			msgEnded(int fd);
		void			sendMsgToChannel(string message, Channel* channel, int fd);
		
		//Channel Management
		Channel*		channelsMng(string& channelName, int fd);
		void			createNewChannel(string& channelName, string& channelPass, int pass, int i, int fd);
		void			existingChannel(Channel* found, string& channelPass, string& channelName, int i, int fd, int flag);
		Channel*		findChannel(string channelName);
		bool			channelConnStatus(int fd, Channel *found, string& channelPass, string& channelName);
		Client*			findNickname(string nick, Channel* channel);
		void			joinMsg(Channel *channel, int fd);
		void			removeChannel(string channelname);

		
		//Getters
		string			getPassword();
		Client			*getClient(int fd);
		Client			*getClientNickname(std::string nickname);
		Client*			getUser(string clientname);

		// Time
		void			setCreationTime() { _creationTime = getCurrentDataTime(); };
		string			getCreationTime() { return _creationTime; };

		//MODE
		void			modeManagement(Channel* channel, std::vector<string>& cmd, int fd);
		bool			isModeCmdValid (Channel* channel, std::vector<string>& cmd, int fd);
		bool			checkModeFlags(Channel* channel, std::vector<string>& cmd, int fd);
		bool			isFlagMode(Channel* channel, std::vector<string>& cmd, int num);
		void			flagModeI(bool flag, Channel *channel);
		void			flagModeT(bool flag, Channel *channel);
		void			flagModeO(bool flag, Channel* channel, string cmd);
		void			flagModeK(bool flag, Channel* channel, string cmd);
		void			flagModeL(bool flag, Channel* channel, string cmd);
		bool			validFlags(string cmd);
		bool 			isNumber(string cmd);

		//isUsed commands
		bool		nickIsUsed(string cmd);
		bool		userIsUsed(string cmd);

		//Commands
		void		passCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		nickCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		userCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		quitCmd(std::vector<string>& cmd, int fd);//ESTA?
		void		modeCmd(std::vector<string>& cmd, int fd);//ESTA?
		void		joinCmd(std::vector<string>& cmd, int fd);//ESTA?
		void		partCmd(std::vector<string>& cmd, int fd);	//ESTA?
		void		topicCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		kickCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		privmsgCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		inviteCmd(std::vector<string>& cmd, int fd);//ESTA?
		void		whoisCmd(std::vector<string>& cmd, int fd);//ESTA?
		void		adminCmd(std::vector<string>& cmd, int fd);//ESTA?
		void		infoCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		pongCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		pingCmd(std::vector<string>& cmd, int fd); //ESTA?
		void		capCmd(std::vector<string>& cmd, int fd);  //ESTA?
};

#endif
