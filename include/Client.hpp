#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"
#include "Channel.hpp"
//#include "MessageHandler.hpp"

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
		//bool					_isOper;
		string					_IPv6;
		string					_nickname;
		string					_username;
		string					_realname;
		string					_hostname;		// maybe we need to print
		string					_msg;			// To manage messages
		State					_state;			// Connection state
		std::vector<Channel*>	_channels;		// Not sure we need that
		//std::vector<Client>	_clients;		// List of all clients

		//string				_password;		// HACE FALTA ?*/
		struct sockaddr_in6		address;		// DE QUE SIRVE ??
		//struct pollfd			*fds;
		bool					_registered;
		//bool					_authentificated;

	public:
		Client();
		Client(int client_fd);
		~Client();
		Client(const Client &src);
		Client &operator=(const Client &src);
		
		//void			newClient(int &numfd);
		//GETTERS
		struct sockaddr_in6	getAddr(); // PARA QUE LO NECESITAMOS ?
		int				getFd();
		string			getNickname() const;
		string			getUsername() const;
		string			getRealname() const;
		string			getHostname() const;
		string			getMsg() const; //tengo que implementarlo
		string			&getMsgRef();
		string			getPrefix() const;
		bool			getIsOper() const;
		State			getState() const;
		const std::vector<Channel*> &getChannels() const;
		bool			clientMaxChannel() const;
		
		//SETTERS
		void setNickname(const string& nickname);
		void setUsername(const string& username);
		void setRealname(const string& realname);
		void setHostname(const string& hostname);
		void setMsg(const string& newMsg);
		void setState(State newState);
		void setIsOper(bool isOper);

		//FUNCTIONS
		void welcome(Server& Server, Client &Client, int fd);          // Need to implement
		void reply(string msg);    //Need to implement*/
		//void	clearSpecMsg();		//Not used NOW
		void	cleanBuff();
		void	addChannel(Channel* channel);
		void	appendToMsg(const string &msg);
};

#endif
