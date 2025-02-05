#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <set>

#include "Client.hpp"

/* */

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
        std::string             _password;       // password required to join the channel
};

#endif
