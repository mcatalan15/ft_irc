#include "../include/Irc.hpp"

void cleanexit(int signal)
{
	(void) signal;
	std::cout << "\b\b\r   Server disconnected - See you soon!\n\n";
	std::cout << "\033[?25h";
	std::system("stty echo");
	exit(0);
}

Server::Server(int port, string password)
{
	this->fd = socket(AF_INET6, SOCK_STREAM, 0); //creates listening FD for the server
	if (this->fd < 0)
		std::cerr << "error: socket connection" << std::endl;
	this->password = password;
	this->opt = 1;

	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(this->opt)) < 0)
		std::cerr << "error: setstockopt error" << std::endl;
	
	this->address.sin6_family = AF_INET6;
	this->address.sin6_addr = in6addr_any;
	this->address.sin6_port = htons(port);
	signal(SIGINT,cleanexit); 
	if (bind(this->fd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0)
		std::cerr << "error: binding error" << std::endl;
	if (listen(this->fd, 5) < 0)
		std::cerr << "error: server not listening" << std::endl;
}



void	Server::client_process()
{
	//Client		client;
	socklen_t	addrlen = sizeof(this->address);
	int		client_fd = accept(this->fd, (struct sockaddr*)&this->address, &addrlen);
	char		buffer[512];

	while (true)
	{
		memset(buffer, 0, 512);
		int	bytes_read = recv(client_fd, buffer, 512, 0);
		if (bytes_read <= 0)
		{
			std::cout << "client disconnected" << std::endl;
			break ;
		}
	std::cout << "Client: " << buffer << std::endl;
	}
	close(client_fd);
	close(this->fd);
}
