#include "../include/Server.hpp"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

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

	if ((_serverFd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) //creates listening FD for the server
		std::cerr << "error: socket connection" << std::endl;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)) < 0)
		std::cerr << "error: setstockopt error" << std::endl;
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
		std::cerr << "error: failed to set option (O_NONBLOCK) on socket" << std::endl;
	if (bind(_serverFd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
		std::cerr << "error: binding error" << std::endl; //bind connecta al puerto
	if (listen(_serverFd, 5) < 0) // deja abierto el puerto
		std::cerr << "error: server not listening" << std::endl;

	s_poll.fd = _serverFd;
	s_poll.events = POLLIN; //cada input al terminal
	s_poll.revents = 0;
	_pollFds.push_back(s_poll);
	std::cout << "Welcome to the FT_IRC" << "\n" << std::endl;	
}

/*
	Destructor
	This destructor iterates through all the fds(clients) connected/opened and close the fd in order to finalize the connection
	with the server
*/
Server::~Server() {
	for (size_t i = 0; i < _pollFds.size(); i++)
		close(_pollFds[i].fd);
	close(_serverFd);
}

/*
	Signal Handler
	This function is used when a signal is detected. Sets the privat variable _signal to true, this way the infinite loop
	is stoped and with that the ircserv.
*/
void Server::signalHandler(int signum) {
	(void)signum;
	std::cout << "Signal received, shutting down..." << std::endl;
	_signal = true;
}

/*
	New client
	This function  is used when the fd[0] (the server) is detected with an event (a new client whants to connect).
	Creates
*/
void Server::new_client(int &numfd) {    
	struct pollfd		newpoll;
	struct sockaddr_in	client_addr; // Struct needed to save the client address.
	socklen_t client_len = sizeof(client_addr); // The size of the struct for the address. 
	int client_fd = accept(_serverFd, (struct sockaddr*)&client_addr, &client_len); // Accepts the connection and recives the fd of the client
	if (client_fd == -1) {
		std::cerr << "accept() failed" << std::endl;
		return;
	}
	std::cout << "clientfd: " << client_fd << std::endl;
	Client	newClient(client_fd);
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "fcntl failed" << std::endl;
	}
	newpoll.fd = client_fd; // Adds the new client fd to the list of monitored fd's.
	newpoll.events = POLLIN; // Marks the fd
	_pollFds.push_back(newpoll);
	_clients.push_back(newClient);
	numfd++;

	std::cout << GREEN << "Client <" << client_fd << "> Connected" << WHITE << std::endl;
}

// If client exists
void	Server::client_exist(int fd) {
	char buffer[512];
	std::memset(buffer, 0, sizeof(buffer)); // clean buffer to verify there's nothing from the old buff
	ssize_t	bytes_read = recv(fd, buffer, sizeof(buffer), 0); //function to read buff
	Client *Client = getClient(fd);
	std::vector<string>	command;
	Client->setHostname(addHostname());
	std::cout << "hostname <" << Client->getHostname() << ">" << std::endl;
	if (bytes_read <= 0) { // Client disconnected or error occurred
		std::cout << "Client disconnected, fd: " << fd << std::endl;
		close(fd);

		// Remove the client from the monitored list
		// removeClientChannel(fd);
		removeClient(fd);
		removeFd(fd);
	} else {
		// Append received data to the client's existing buffer
		string received_data(buffer, bytes_read);
		Client->appendToMsg(received_data);

		// Process the buffer while it contains complete messages ending with "\r\n"
		string &msg_buffer = Client->getMsgRef();
		size_t pos;
		while ((pos = msg_buffer.find("\r\n")) != string::npos) {
			// Extract a single complete message
			string complete_msg = msg_buffer.substr(0, pos);
			msg_buffer.erase(0, pos + 2);
			/*
			!!!!!!!!!!!!!!!!!!!!!!!!
			AQUI VA LA MANDANGA
			!!!!!!!!!!!!!!!!!!!!!!!!!
			*/
			std::cout << "////////////////////////////////////////" << std::endl;
			std::cout << "msg_buffer: <" << complete_msg << ">" << std::endl;
			Client->setMsg(complete_msg);
			//Client->clearSpecMsg();
			msgManagement(fd);
			if (getClient(fd)) // to delete the _msg once is used
				Client->cleanBuff();
		}
	}
}

void	Server::client_process() {
	int numfd = 1; //empieza en 1 pq server es 0
	while (!(this->_signal)) {
		int numEvents = poll(&_pollFds[0], numfd, -1); //
		if (numEvents < 0 && _signal == false)
			std::cerr << "polling failed" << std::endl;

		for (int i = 0; i < numfd; i++) { //si hay eventos entra (si hay clientes conectados)
			if (_pollFds[i].revents & POLLIN) { //mira si el evento (cliente) es de input (POLLIN) 
				if (_pollFds[i].fd == _serverFd) //mira si el evento es alguien nuevo?
					new_client(numfd);
				else 
					client_exist(_pollFds[i].fd);
			}
		}
	}
	closeFds();
}

// Getters
string	Server::getPassword() { return this->_password; }

Client *Server::getClient(int fd) {
	for (size_t i = 0; i < this->_clients.size(); i++) {
		if (this->_clients[i].getFd() == fd)
			return &this->_clients[i];
	}
	return NULL;
}

void Server::closeFds() {
	for (size_t i = 1; i < _pollFds.size(); i++) {
		std::cout << "Client <" << _pollFds[i].fd << "> Disconnected" << std::endl;
		string message = "Server has disconnected\n";
		send(_pollFds[i].fd, message.c_str(), message.size(), 0);
		close(_pollFds[i].fd);
	}
	if (_serverFd != -1) {
		std::cout << "Server <" << _serverFd << "> Disconnected" << std::endl;
		close(_serverFd);
	}
}

void	Server::removeClient(int fd) {
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getFd() == fd) {
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}

void	Server::removeFd(int fd) {
	for (size_t i = 0; i < _pollFds.size(); i++) {
		if (_pollFds[i].fd == fd) {
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}
}

// Define the map outside the function
const std::map<std::string, void (Server::*)(std::vector<string>&, int)> Server::cmdMap = Server::createCmdMap();

std::map<std::string, void (Server::*)(std::vector<string>&, int)> Server::createCmdMap() {
	std::map<std::string, void (Server::*)(std::vector<string>&, int)> map;
	map["CAP"] = &Server::capCmd;
	map["PASS"] = &Server::passCmd;
	map["NICK"] = &Server::nickCmd;
	map["USER"] = &Server::userCmd;
	map["QUIT"] = &Server::quitCmd;
	map["MODE"] = &Server::modeCmd;
	map["JOIN"] = &Server::joinCmd;
	map["PART"] = &Server::partCmd;
	map["TOPIC"] = &Server::topicCmd;
	map["KICK"] = &Server::kickCmd;
	map["PRIVMSG"] = &Server::privmsgCmd;
	map["INVITE"] = &Server::inviteCmd;
	map["WHOIS"] = &Server::whoisCmd;
	map["ADMIN"] = &Server::adminCmd;
	map["INFO"] = &Server::infoCmd;
	map["PONG"] = &Server::pongCmd;
	map["PING"] = &Server::pingCmd;
	return map;
}

void	Server::msgManagement( int fd) {
	string command = getClient(fd)->getMsg();
	std::cout << "msgManagement <" << command << ">" << std::endl;
	if (command.empty())
		return ;
	
	std::vector<string> cmd = splitMsg(command);
	// Use getCommandInUpper to extract and normalize the command
	string upperCmd = getCommandInUpper(cmd[0]); //toupper
	
	std::map<string, void (Server::*)(std::vector<string>&, int)>::const_iterator it = cmdMap.find(upperCmd);
	if (it != cmdMap.end()) { // Execute the command
		std::cout << "Sale del while!!!!!!!!!!!!!!!!1" << std::endl;
		(this->*(it->second))(cmd, fd);
	} else {
			//CUIDADO PETA!!!!!
			// Unknown command    !!!!!! VERIFICAR SI NO ES ERR_CMDNOTFOUND
		sendMsg(ERR_UNKNOWNCOMMAND(getClient(fd)->getNickname(), cmd[0]), fd);
	}
}

bool Server::isRegistered(int fd) {
	//
	if (getClient(fd)->getState() == REGISTERED)
		return true;
	return false;
}

void Server::sendMsg(string msg, int fd) {
	send(fd, msg.c_str(), msg.size(), 0);
}
