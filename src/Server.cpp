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

	this->epoll_fd = epoll_create1(0);

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = this->fd;

	epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->fd, &event);
}

void	Server::client_process()
{
	struct	epoll_event	events[1024];
	int			numEvents;
	int			client_fd;

	while (true)
	{
		numEvents = epoll_wait(this->epoll_fd, events, 1024, -1);
		if (numEvents < 0)
		{
			std::cerr << "error: waiting events" << std::endl;
			break ;
		}

		for (int i = 0; i < numEvents; i++)
		{
			if (events[i].data.fd == this->fd)
			{
				struct	sockaddr_in6	client_address;
				socklen_t		addrlen = sizeof(client_address);

				client_fd = accept(this->fd, (struct sockaddr*)&client_address, &addrlen);
				if (client_fd < 0)
					std::cerr << "error: didnt accept client" << std::endl;

				struct epoll_event	client_event;

				client_event.events = EPOLLIN;
				client_event.data.fd = client_fd;

				if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) < 0)
					std::cerr << "error: failed to add client" << std::endl;
			}
			else
			{
				char	buffer[512];

				client_fd = events[i].data.fd;
				memset(buffer, 0, 512);

				int	bytes_read = recv(client_fd, buffer, 512, 0);
				if (bytes_read <= 0)
				{
					std::cout << "client disconnected" << std::endl;
					close(client_fd);
					epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
					break ;
				}
				else
					std::cout << "Client: " << buffer << std::endl;
			}
		}
	}
	close(this->fd);
	close(this->epoll_fd);
}
