#include "../include/Irc.hpp"
#include <cstddef>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>

bool	Server::_signal = false;

/*
	Constructor
	This constructor
*/
Server::Server(int port, string password) {
	struct pollfd   s_poll;

	_address.sin6_family = AF_INET6; // sets IP path to ipv6
	_address.sin6_addr = in6addr_any; // accepts any IP
	_address.sin6_port = htons(port); // gets port number in bytes (abre puerto input)

	_password = password; //contrasena del input
	_opt = 1; //opciones que se le puede dar al server en setsocketopt

	if ((_fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) //creates listening FD for the server
		std::cerr << "error: socket connection" << std::endl;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)) < 0)
		std::cerr << "error: setstockopt error" << std::endl;
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
		std::cerr << "error: failed to set option (O_NONBLOCK) on socket" << std::endl;
	if (bind(_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
		std::cerr << "error: binding error" << std::endl; //bind connecta al puerto
	if (listen(_fd, 5) < 0) // deja abierto el puerto
		std::cerr << "error: server not listening" << std::endl;

	s_poll.fd = _fd;
	s_poll.events = POLLIN; //cada input al terminal
	_fds.push_back(s_poll);
	std::cout << "Welcome to the FT_IRC" << "\n" << std::endl;	
}

/*
	Destructor
	This destructor iterates through all the fds(clients) connected/opened and close the fd in order to finalize the connection
	with the server
*/
Server::~Server() {
	for (size_t i = 0; i < _fds.size(); i++)
		close(_fds[i].fd);
	close(_fd);
}

/*
	Signal Handler
	This function is used when a signal is detected. Sets the privat variable _signal to true, this way the infinite loop
	is stoped and with that the ircserv.
*/
void Server::signalHandler(int signum) {
	(void)signum;
	_signal = true;
}

/*
	New client
	This function  is used when the fd[0] (the server) is detected with an event (a new client whants to connect).
	Creates
*/
void Server::new_client(int &numfd) {    
	//Client  newClient(_fd);
	struct pollfd		newpoll;
	struct sockaddr_in	client_addr; // Struct needed to save the client address.
	socklen_t client_len = sizeof(client_addr); // The size of the struct for the address. 
	int client_fd = accept(_fd, (struct sockaddr*)&client_addr, &client_len); // Accepts the connection and recives the fd of the client

	std::cout << "clientfd: " << client_fd << std::endl;
	//_fds[i].fd = newClient.getFd();
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	newpoll.fd = client_fd; // Adds the new client fd to the list of monitored fd's.
	newpoll.events = POLLIN; // Marks the fd
	_fds.push_back(newpoll);
	numfd++;
}

// If client exists
void	Server::client_exist(int &numfd, int i) {
	char buffer[512] = {};
	int bytes_read = recv(_fds[i].fd, buffer, 512, 0);

	if (bytes_read <= 0) {
	// Client disconnected or error occurred
	std::cout << "Client disconnected, fd: " << _fds[i].fd << std::endl;
	close(_fds[i].fd);
 
	// Remove the client from the monitored list
	_fds[i] = _fds[numfd - 1]; // Replace with the last active descriptor
	numfd--;
	} else {
	// Print received message
	std::cout << "Client (" << i << "): " << buffer << std::endl;
	// Echo the message back to the client
	send(_fds[i].fd, buffer, bytes_read, 0);
	}
}

void	Server::client_process() {
	int numfd = 1; //empieza en 1 pq server es 0
	while (!(this->_signal)) {
		int numEvents = poll(&_fds[0], numfd, -1); //
		if (numEvents < 0 && _signal != true)
			std::cerr << "polling failed" << std::endl;

		for (int i = 0; i < numfd; i++) 
		{ //si hay eventos entra (si hay clientes conectados)
			if (_fds[i].revents & POLLIN) 
			{ //mira si el evento (cliente) es de input (POLLIN) 
				if (_fds[i].fd == _fd) //mira si el evento es alguien nuevo?
					new_client(numfd);
				else 
					client_exist(numfd, i);
			}
		}
	}
	std::cout << "size "<< _fds.size() << std::endl;
	for (size_t i = 1; i < _fds.size(); ++i) { // Start from 1 to skip the server socket
		std::string shutdown_message = "Server is shutting down for client\n";

		send(_fds[i].fd, shutdown_message.c_str(), shutdown_message.size(), 0);
		std::cout << "Closing client fd: " << _fds[i].fd << std::endl;
		close(_fds[i].fd);
	}
	_fds.clear(); // Clear the entire vector after cleanup
}
