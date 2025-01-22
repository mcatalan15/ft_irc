#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <set>

#include "Client.hpp"

/* */

class Channel
{
    public:
        Channel(const std::string& name);
        ~Channel();

        std::string getName() const;

        // Client management
        std::vector<Client*> &getClients() const;
        void addClient(Client* client);
        void removeClient(Client* client);
        bool hasClient() const;

        // Password management
        std::string getPassword() const;
        void setPassword(const std::string& password);
        bool hasPassword() const;

        // User limit managent
        int getUserLimit() const;
        int setUserLimit(int limit);
        bool isFull() const;

        // Operator management
        std::set<Client*> &getOperators() const;
        void addOperator(Client* client);
        void removeOperator(Client* client);
        bool isOperator(Client *client) const;

        // Invitation management
        std::set<Client*> &getInvitedClients() const;
        void addInvitation(Client *client);
        void removeInvitation(Client *client);
        bool isInvited(Client *client) const;

    private:
        std::string             _name;          // name of the channel
        std::vector<Client*>    _client;        // list of clients in the channel
        int                     _userLimit;     // max num of clients in the channel
        std::set<Client*>       _operators;     // set of operators in the channel
        std::set<Client*>       _invitedClients; // set of clients invited to the channel
        std::string             _password;       // password required to join the channel
};

#endif
