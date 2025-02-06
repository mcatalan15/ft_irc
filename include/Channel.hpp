#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <set>

#include "Client.hpp"

/* 
MODES
+/- i -> invite only (default: off = -i)
+/- t -> topic restricted (default: off = -t)
+/- k -> password set (default: off = -k) if pass is set Change to +k  
+/- l -> user limit (default: off = -l)
*/
class Client;

class Channel
{
	public:
    	enum Mode {
		    INVITE_ONLY = (1 << 0), 	//0001 (+i/-i) -> 1
		    TOPIC_RESTRICTED = (1 << 1),//0010 (+t/-t) -> 2
		    PASSWORD_SET = (1 << 2),	//0100 (+k/-k) -> 4
		    USER_LIMIT = (1 << 3)		//1000 (+l/-l) -> 8
	    };
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

        /*---------- CHANNEL OPERATORS ---------------*/
        const std::set<Client*> &getOperators() const;
        void addOperator(Client* client);
        void removeOperator(Client* client);
        bool isOperator(Client *client) const;


        /*---------- CHANNEL LIMITS ---------------*/
        int getUserLimit() const;
        int setUserLimit(int limit);
        bool isFull() const;


        /*----------- CHANNEL INVITATIONS ----------*/
        const std::set<Client*> &getInvitedClients() const;
        void addInvitation(Client *client);
        void removeInvitation(Client *client);
        bool isInvited(Client *client) const;

    private:
        std::string             _name;          // name of the channel
        string					_topic;
        std::vector<Client*>    _clients;        // list of clients in the channel
        size_t                  _userLimit;     // max num of clients in the channel
        std::set<Client*>       _operators;    // set of operators in the channel
        std::set<Client*>       _invitedClients; // set of clients invited to the channel
        std::string             _password; // password required to join the channel
       // t_modes					_mode;
};

#endif
