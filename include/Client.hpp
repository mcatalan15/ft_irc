#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <sys/poll.h>

class Client{
    private:
        int         fd;
        //string 			nickname;
        //string  		name;
	//string			password;
	struct sockaddr_in6     address;
	//struct pollfd      *fds;
	bool    registered;

    public:
	Client(int server_fd);

	struct sockaddr	getAddr();
	int             getFd();
};

#endif
