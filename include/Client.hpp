#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <csignal>
#include <sys/poll.h>
#include <string>
//#include "MessageHandler.hpp"

// Definition of possible client state
// We can move it to Irc.hpp maybe ?
enum State
{
    HANDSHAKE,  // Initial conection state
    LOGIN,      // The client has sent NICK and USER, but is mnot yet registred
    REGISTERED  // The client is fully registered and ready to interact
};

class Client
{
    private:
        int                     _fd;
        /*std::string 	        _nickname;
        std::string  	        _username;
        std::string             _realname;
        std::string             _hostname;      // maybe we need to print
        std::string             _msg;           // To manage messages
        bool                    _isOper;        // Is the client an IRC operator ?
        State                   _state;         // Connection state
        //std::vector<Channel>    _channels;      // Not sure we need that

        //std::string	_password;   // HACE FALTA ?*/
        struct sockaddr_in6     address; // DE QUE SIRVE ??
	//struct pollfd      *fds;
	    bool   _registered;
	//bool   _authentificated;

    public:
        Client(int server_fd);
        ~Client();

        //GETTERS
        struct sockaddr	getAddr(); // PARA QUE LO NECESITAMOS ?
        int             getFd();
        /*std::string     getNickname() const;
        std::string     getUsername() const;
        std::string     getRealname() const;
        std::string     getHostname() const;
        std::string     getMsg() const; //tengo que implementarlo
        std::string     getPrefix() const;
        bool            getIsOper() const;
        State           getState() const;

        //SETTERS
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& username);
        void setRealname(const std::string& realname);
        void setHostname(const std::string& hostname);
        void setMsg(const std::string& newMsg);
        void setState(State newState);
        void setIsOper(bool isOper);

        //FUNCTIONS
        void welcome();                 // Need to implement
        void reply(std::string msg);    //Need to implement*/
};

#endif
