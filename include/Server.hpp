#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;

class Server {
	private:
		int							_serverFd;
		int							_opt;
		static bool					_signal;
		std::vector<Client>			_clients;
		std::vector<struct pollfd>	_pollFds;
		struct sockaddr_in6			_address;
		string						_password;
		std::vector<Channel>		_channels;
		string						_creationTime;
		time_t						_creationTimeT;
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
		void			remove_fd(int fd);

		//Msg management
		void			msgManagement(int fd);
		bool			msgEnded(int fd);
		void			sendMsgToChannel(string message, Channel* channel, int fd);
		void			sendMsgToClients(string message, std::vector<string> channelnames, int fd);

		//Channel Management
		Channel*		channelsMng(string& channelName);
		void			createNewChannel(string& channelName, string& channelPass, int pass, int i, int fd);
		void			existingChannel(Channel* found, string& channelPass, string& channelName, int fd, int flag);
		Channel*		findChannel(string channelName);
		bool			channelConnStatus(int fd, Channel *found, string& channelPass, string& channelName);
		Client*			findNickname(string nick, Channel* channel);
		void			joinMsg(Channel *channel, int fd);
		void			removeChannel(string channelname);
		bool			alreadyJoined(Channel* channel, string user);

		//Topic
		bool			isOnChan(string& cmd, int fd);
		void			topicDisplay(string& cmd, int fd);
		void			topicSetter(std::vector<string>& cmd, int fd);

		//Getters
		string			getPassword();
		Client			*getClient(int fd);
		Client			*getClientNickname(std::string nickname);
		Client*			getUser(string clientname);
		Client*			getNick(string clientname);
		Channel*		getChannel(string channelName);

		std::vector<Client> &getClients();

		// Time
		void			setCreationTime() { _creationTime = getCurrentDataTime(); };
		void			setCreationTimeT() {  time_t now = time(0); _creationTimeT = now; };
		string			getCreationTime() { return _creationTime; };
		time_t			getCreationTimeT() { return _creationTimeT; };
		
		std::vector<string>	getInfo(void);

		//MODE
		void			modeManagement(Channel* channel, std::vector<string>& cmd, string modeChar, int fd);
		bool			isModeCmdValid (Channel* channel, std::vector<string>& cmd, int fd);
		bool			checkModeFlags(string modeChar, int fd);
		bool			isFlagMode(char modeChar, int num, int fd);
		void			flagModeI(bool flag, Channel *channel, int fd);
		void			flagModeT(bool flag, Channel *channel, int fd);
		void			flagModeO(bool flag, Channel* channel, string cmd, int fd);
		void			flagModeK(bool flag, Channel* channel, string cmd, int fd);
		void			flagModeL(bool flag, Channel* channel, string cmd, int fd);
		bool			validFlags(Channel* channel, string modeChar, int fd, bool sign);
		bool 			isNumber(string cmd);
		void			sendModeGeneralMsg(Channel *channel, string param, string target, int fd);
		void			modeTypeD(Channel *channel, char modeChar, bool flag, int fd);
		void			modeTypeC(Channel *channel, char modeChar, string param, bool flag, int fd);

		//INVITE
		void			invitationManagement(Channel* channel, std::vector<string>& nickName, int fd, bool flag);
		bool			userOnChannel(Channel* channel, std::vector<string> nickName, int fd);
		bool			nicknameExist(std::vector<string> nickName, int fd);
		bool			findNicknameOnServer(string nickName, int fd);
		std::vector<string>	divisor(string cmd, bool flag);
		bool			isInviteCmdValid(Channel* channel, std::vector<string>& cmd, int fd);
		void            sendInvitationMsg(Channel *channel, string nickName, int fd);

		//isUsed commands
		bool		nickIsUsed(string cmd);
		bool		userIsUsed(string cmd);
		
		//INFO cmd
		string		getActiveClients(void);
		string		getActiveChannels(void);

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
		void		infoCmd(std::vector<string>& cmd, int fd);
		void		pingCmd(std::vector<string>& cmd, int fd);
		void		capCmd(std::vector<string>& cmd, int fd);
};

#endif
