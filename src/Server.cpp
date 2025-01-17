#include "../include/Irc.hpp"
#include <sys/poll.h>
#include <sys/socket.h>

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
	
	this->address.sin6_family = AF_INET6; // sets IP path to ipv6
	this->address.sin6_addr = in6addr_any; // accepts any IP
	this->address.sin6_port = htons(port); // gets port number in bytes

	signal(SIGINT,cleanexit);

	if (bind(this->fd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0)
		std::cerr << "error: binding error" << std::endl;
	if (listen(this->fd, 5) < 0)
		std::cerr << "error: server not listening" << std::endl;
		
	this->fds[0].fd = this->fd;
	this->fds[0].events = POLLIN;
}

void	Server::client_process()
{
    int numfd = 1;
    while (true)
    {
        int numEvents = poll(this->fds, numfd, -1);
        if (numEvents < 0)
            std::cerr << "polling failed" << std::endl;
            
        for (int i = 0; i < numfd; i++)
        {
            if (this->fds[i].revents & POLLIN)
            {
                if (this->fds[i].fd == this->fd)
                {
                    //Client  newClient(this->fd);
                    struct sockaddr_in client_addr;
                      socklen_t client_len = sizeof(client_addr);
                      int client_fd = accept(this->fd, (struct sockaddr*)&client_addr, &client_len);

                    
                    //this->fds[i].fd = newClient.getFd();
                    this->fds[i].fd = client_fd;
                    this->fds[i].events = POLLIN;
                    numfd++;
                }
                else 
                {
                    char buffer[512] = {};
                     int bytes_read = recv(this->fds[i].fd, buffer, 512, 0);
 
                     if (bytes_read <= 0)
                     {
                         // Client disconnected or error occurred
                         std::cout << "Client disconnected, fd: " << fds[i].fd << std::endl;
                         close(this->fds[i].fd);
 
                         // Remove the client from the monitored list
                         fds[i] = fds[numfd - 1]; // Replace with the last active descriptor
                         numfd--;
                     } 
                     else
                     {
                         // Print received message
                         std::cout << "Client (" << fds[i].fd << "): " << buffer << std::endl;
 
                         // Echo the message back to the client
                         send(fds[i].fd, buffer, bytes_read, 0);
                     }
                }
            }
        }
    }
}
