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
		Channel(const string& name);
		Channel(const Channel& cpy);
		~Channel();

		/*------------ CHANNEL INFO ---------------*/
		const string getName() const;

		// Password management
		string getPassword() const;
		void setPassword(const string& password);
		bool hasPassword() const;

		/*---------- CHANNEL MANAGEMENT -----------*/
		// Client management
		const std::vector<string> &getClients() const;
		void addClient(string clientname);
		void removeClient(string clientname);
		bool hasClient(string clientname) const;
		Client*	findNickname(string nick);

		/*---------- CHANNEL OPERATORS ---------------*/
		const std::vector<string> &getOperators() const;
		void addOperator(string clientname);
		void removeOperator(string clientname);
		bool isOperator(string clientname) const;
		void	setMode(Mode mode);
		void	unsetMode(Mode mode);
		bool	isModeSet(Mode mode);


		/*---------- CHANNEL LIMITS ---------------*/
		int getUserLimit() const;
		int setUserLimit(int limit);
		bool isFull() const;


		/*----------- CHANNEL INVITATIONS ----------*/
		const std::vector<string> &getInvitedClients() const;
		const std::vector<string> &getBannedClients() const;
		void addBannedClient(string clientname);
		void addInvitation(string clientname);
		void removeInvitation(string clientname);
		bool isInvited(string clientname) const;
		bool isBanned(string clientname) const;

		/*----------CHANNEL TIME ------------------ */
		void	setCreationTime() { _creationTime = getCurrentDataTime(); };
		string	getCreationTime() { return _creationTime; };

		private:
			string					_name;          // name of the channel
			string					_topic;
			std::vector<string>		_clients;       // list of clients in the channel
			size_t					_userLimit;   	// max num of clients in the channel
			std::vector<string>		_operators;     // set of operators in the channel
			std::vector<string>		_invitedClients; // set of clients invited to the channel
			std::vector<string>		_bannedClients; // set of clients banned 
			string					_password; // password required to join the channel
			unsigned int			_modes;
			string					_creationTime;
};

#endif
