#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <set>

#include "Client.hpp"
#include "Irc.hpp"

/* 
MODES
+/- i -> invite only (default: off = -i)
+/- t -> topic restricted (default: off = -t)
+/- k -> password set (default: off = -k) if pass is set Change to +k  
+/- l -> user limit (default: off = -l)
*/
enum Mode {
	INVITE_ONLY = (1 << 0), 	//0001 (+i/-i) -> 1
	TOPIC_RESTRICTED = (1 << 1),//0010 (+t/-t) -> 2
	PASSWORD_SET = (1 << 2),	//0100 (+k/-k) -> 4
	USER_LIMIT = (1 << 3)		//1000 (+l/-l) -> 8
};

class Client;

class Channel
{
	public:
		Channel(const std::string& name);
		Channel(const Channel& cpy);
		~Channel();

		/*------------ CHANNEL INFO ---------------*/
		const std::string getName() const;

		// Password management
		std::string getPassword() const;
		void setPassword(const std::string& password);
		bool hasPassword() const;

		/*---------- CHANNEL MANAGEMENT -----------*/
		// Client management
		const std::vector<Client*> &getClients() const;
		void addClient(Client* client);
		void removeClient(Client* client);
		bool hasClient(Client *client) const;
		Client*	findNickname(string nick);

		/*---------- CHANNEL OPERATORS ---------------*/
		const std::set<Client*> &getOperators() const;
		void addOperator(Client* client);
		void removeOperator(Client* client);
		bool isOperator(Client *client) const;
		void	setMode(Mode mode);
		void	unsetMode(Mode mode);
		bool	isModeSet(Mode mode);


		/*---------- CHANNEL LIMITS ---------------*/
		int getUserLimit() const;
		int setUserLimit(int limit);
		bool isFull() const;


		/*----------- CHANNEL INVITATIONS ----------*/
		const std::set<Client*> &getInvitedClients() const;
		const std::set<Client*> &getBannedClients() const;
		void addBannedClient(Client *client);
		void addInvitation(Client *client);
		void removeInvitation(Client *client);
		bool isInvited(Client *client) const;
		bool isBanned(Client *client) const;

		/*----------CHANNEL TIME ------------------ */
		void	setCreationTime() { _creationTime = getCurrentDataTime(); };
		string	getCreationTime() { return _creationTime; };

		private:
			std::string				_name;          // name of the channel
			string					_topic;
			std::vector<Client*>	_clients;        // list of clients in the channel
			size_t					_userLimit;     // max num of clients in the channel
			std::set<Client*>		_operators;    // set of operators in the channel
			std::set<Client*>		_invitedClients; // set of clients invited to the channel
			std::set<Client*>		_bannedClients; // set of clients banned 
			std::string				_password; // password required to join the channel
			unsigned int			_modes;
			string					_creationTime;
};

#endif
