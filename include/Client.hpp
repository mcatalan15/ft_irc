#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

class Client{
    private:
        string 			nickname;
        string  		name;
	string			password;
	//struct sockaddr_in6     address;
       // bool    registered;

    public:
	Client();

	struct sockaddr	getAddr();

};

#endif
