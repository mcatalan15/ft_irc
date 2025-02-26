#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"
#include "Channel.hpp"

// Definition of possible client state
// We can move it to Irc.hpp maybe ?
enum State {
	HANDSHAKE,  // Initial conection state
	NICK,
	LOGIN,      // The client has sent NICK and USER, but is mnot yet registred
	REGISTERED  // The client is fully registered and ready to interact
};

class Channel;

class Server;

class Client
{
	private:
		int						_fd;
		string					_IPv6;
		string					_nickname;
		string					_username;
		string					_realname;
		string					_hostname;		// maybe we need to print
		string					_msg;			// To manage messages
		State					_state;			// Connection state
		std::vector<string>		_channels;
		struct sockaddr_in6		address;		// DE QUE SIRVE ??
		bool					_registered;
		
	public:
		Client();
		Client(int client_fd);
		~Client();
		Client(const Client &src);
		Client &operator=(const Client &src);

		//GETTERS
		struct sockaddr_in6			getAddr();
		int							getFd();
		string						getNickname() const;
		string						getUsername() const;
		string						getRealname() const;
		string						getHostname() const;
		string						getMsg() const;
		string						&getMsgRef();
		string						getPrefix() const;
		bool						getIsOper() const;
		State						getState() const;
		const std::vector<string> 	&getChannels() const;
		bool						clientMaxChannel() const;

		//SETTERS
		void 						setNickname(const string& nickname);
		void 						setUsername(const string& username);
		void 						setRealname(const string& realname);
		void 						setHostname(const string& hostname);
		void 						setMsg(const string& newMsg);
		void 						setState(State newState);
		void 						setIsOper(bool isOper);

		//FUNCTIONS
		void 						welcome(Server& Server, Client &Client, int fd);          // Need to implement
		void 						reply(string msg);    //Need to implement*/
		void						cleanBuff();
		void						addChannel(string channelname);
		bool						removeChannel(string channelname);
		void						appendToMsg(const string &msg);
};

#endif
