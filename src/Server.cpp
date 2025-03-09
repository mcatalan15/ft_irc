#include "../include/Server.hpp"
#include <stdexcept>

bool	Server::_signal = false;

Server::Server(int port, string password) {
	struct pollfd   s_poll;

	memset(&_address, 0, sizeof(_address)); //Avoid unitialized memory errors
	_address.sin6_family = AF_INET6; // sets IP path to ipv6
	_address.sin6_addr = in6addr_any; // accepts any IP
	_address.sin6_port = htons(port); // gets port number in bytes (abre puerto input)

	_password = password; //contrasena del input
	_opt = 1; //opciones que se le puede dar al server en setsocketopt

	if ((_serverFd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) //creates listening FD for the server
		throw std::runtime_error("error: socket connection");
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)) < 0)
		throw std::runtime_error("error: setstockopt error");
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("error: failed to set option (O_NONBLOCK) on socket");
	if (bind(_serverFd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
		throw std::runtime_error("error: binding error"); //bind connecta al puerto
	if (listen(_serverFd, MAX_CONNECTIONS) < 0) // deja abierto el puerto
		throw std::runtime_error("error: server not listening");

	s_poll.fd = _serverFd;
	s_poll.events = POLLIN; //cada input al terminal
	s_poll.revents = 0;
	_pollFds.push_back(s_poll);
	setCreationTime();
	setCreationTimeT();
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
	struct sockaddr_in		client_addr; // Struct needed to save the client address.
	socklen_t client_len = 	sizeof(client_addr); // The size of the struct for the address. 
	int client_fd = 		accept(_serverFd, (struct sockaddr*)&client_addr, &client_len); // Accepts the connection and recives the fd of the client
	
	if (client_fd < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return; // Not a real error, just no clients to accept
		std::cerr << "accept() failed: " << strerror(errno) << std::endl;
	}
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "fcntl failed" << std::endl;
		close(client_fd);
		return ;
	}
	
	Client	newClient(client_fd);
	
	struct pollfd		newpoll = {};
	
	newpoll.fd = client_fd; // Adds the new client fd to the list of monitored fd's.
	newpoll.events = POLLIN; // Marks the fd
	newpoll.revents = 0; // Initialize revents 0
	
	// Ensure the _pollFds has enought space to acommodate the new Fd
	if (static_cast<std::vector<pollfd>::size_type>(numfd) >= _pollFds.size())
		_pollFds.push_back(newpoll); //Add new poll() to the vector
	else
		_pollFds[numfd] = newpoll; // Replace an existance entry (if any)
	_clients.push_back(newClient); // Add client to the Client vector
	numfd++; // Increment the number of Fds
	std::cout << GREEN << "Client <" << client_fd - 3 << "> Connected" << WHITE << std::endl;
}

void	Server::client_exist(int fd) {
	char buffer[1024];
	ssize_t	bytes_read = recv(fd, buffer, sizeof(buffer), 0); //function to read buff
	Client *Client = getClient(fd);
	std::vector<string>	command;
	
	Client->setHostname(addHostname());
	if (bytes_read <= 0) {
		std::cerr << "Client disconnected or read error, fd: " << fd - 3 << std::endl;
		std::vector<string> cmd = joinDivisor("QUIT :leaving");
		quitCmd(cmd, fd);
	} 
	else
	{
		// Append received data to the client's existing buffer
		string received_data(buffer, bytes_read);
		Client->appendToMsg(received_data);
		if (msgEnded(fd)) {
			// Process the buffer while it contains complete messages ending with "\r\n"
			string &msg_buffer = Client->getMsgRef();
			std::vector<string> cmd = splitCommand(msg_buffer);
			for (size_t i = 0; i < cmd.size(); i++) {
				Client->setMsg(cmd[i]);
				msgManagement(fd);
				if (getClient(fd)) // to delete the _msg once is used
					Client->cleanBuff();
			}
		}
	}
}

bool	Server::msgEnded(int fd) {
	if (getClient(fd)->getMsg().find("\r\n") != string::npos)
		return true;
	return false;
}

void	Server::client_process() {
	int numfd = 1; //empieza en 1 pq server socket es 0
	while (!(this->_signal)) {
		//Adjustment numfd to the real size of _pollFds
		numfd = std::min(numfd, static_cast<int>(_pollFds.size())); //std::min returns the min val of 2 values
		
		//Wait for events on Fds
		int numEvents = poll(&_pollFds[0], numfd, -1);
		if (numEvents < 0 && _signal == false) {
			std::cerr << "polling failed" << std::endl;
			continue; //Skip the loop in case of err
		}
		// Iterate through the fds USING vector IT in case _pollFds change inside the for to be updated
		for (size_t i = 0; i < _pollFds.size(); i++) { //si hay eventos entra (si hay clientes conectados)
			if (_pollFds[i].revents & POLLIN) { //mira si el evento (cliente) es de input (POLLIN) 
				if (_pollFds[i].fd == _serverFd) //mira si el evento es alguien nuevo?
					new_client(numfd);
				else 
					client_exist(_pollFds[i].fd);
			}
		}
	}
	closeFds(); //Close all fds when finished
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
	std::vector<int>to_remove;
	
	for (size_t i = 1; i < _pollFds.size(); i++) {
		std::cout << "Client <" << _pollFds[i].fd << "> Disconnected" << std::endl;
		string message = "Server has disconnected\n";
		send(_pollFds[i].fd, message.c_str(), message.size(), 0);
		close(_pollFds[i].fd);
		to_remove.push_back(_pollFds[i].fd);
	}
	for (std::vector<int>::size_type i = 0; i < to_remove.size(); ++i)
		remove_fd(to_remove[i]);
	if (_serverFd != -1) {
		std::cout << "Server <" << _serverFd << "> Disconnected" << std::endl;
		close(_serverFd);
		_serverFd = -1;
	}
}

void	Server::remove_fd(int fd) {
	for (std::vector<struct pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it) {
		if (it->fd == fd) {
			_pollFds.erase(it);
			break;
		}
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

void	Server::removeChannel(string channelname)
{
	std::vector<Channel>::iterator	it;

	for (it = _channels.begin(); it != _channels.end(); it++) {
		if ((*it).getName() == channelname) {
			_channels.erase(it);
			break ;
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
	map["INFO"] = &Server::infoCmd;
	map["PING"] = &Server::pingCmd;
	map["DCC"] = &Server::dccCmd;
	return map;
}

void	Server::msgManagement( int fd) {
	string command = getClient(fd)->getMsg();
	if (command.empty())
		return ;
	std::vector<string> cmd = splitMsg(command);
	if (cmd.empty())
		return ;

	string upperCmd = getCommandInUpper(cmd[0]);

	std::map<string, void (Server::*)(std::vector<string>&, int)>::const_iterator it = cmdMap.find(upperCmd);
	if (it != cmdMap.end())
		(this->*(it->second))(cmd, fd);
	else
		sendMsg(ERR_UNKNOWNCOMMAND(getClient(fd)->getNickname(), cmd[0]), fd);
}

bool Server::isRegistered(int fd) 
{
	if (getClient(fd)->getState() == REGISTERED)
		return true;
	return false;
}

Channel*	Server::findChannel(string channelName)
{
	Channel*	channel = NULL;
	for (size_t i = 0; i < _channels.size() && !channel; i++) {
		if (channelName == _channels[i].getName())
			channel = &_channels[i];
	}
	return channel;
}

void	Server::sendMsgToChannel(string message, Channel* channel, int fd)
{
	if (!channel)
		return ;
	Client*				client = getClient(fd);
	std::vector<string>	clientsVec = channel->getClients();

	for (size_t i = 0; i < clientsVec.size(); i++) {
		if (clientsVec[i] != client->getUsername())
			sendMsg(USER_ID(client->getNickname(), client->getUsername()) + " " + message + CRLF, getUser(clientsVec[i])->getFd());
	}
}

Client*		Server::getUser(string clientname)
{
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getUsername() == clientname)
			return &_clients[i];
	}
	return NULL;
}

Client*		Server::getNick(string clientname) {
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickname() == clientname)
			return &_clients[i];
	}
	return NULL;
}

bool	Server::alreadyJoined(Channel* channel, string user) {
	std::vector<string> list = channel->getClients();
	if (channel) {
		for(size_t i = 0; i < list.size(); i++) {
			if (list[i] == user)
				return true;
		}
	}
	return false;
}

std::vector<Client> &Server::getClients() { return (_clients); }

Client*	Server::findNickname(string nick, Channel* channel)
{
	const std::vector<string>&	lstClients = channel->getClients();
	if (lstClients.size() <= 0)
		return NULL;
	for (size_t i = 0; i < lstClients.size(); i++) {
		if (getUser(lstClients[i])->getNickname() == nick)
			return getUser(lstClients[i]);
	}
	return NULL;
}

void	Server::sendMsgToClients(string message, std::vector<string> channelnames, int fd)
{
	Client*								client = getClient(fd);
	std::vector<string>::const_iterator	it;

	for (size_t i = 0; i < _clients.size(); i++) 
	{
		if (_clients[i].getNickname() != client->getNickname()) 
		{
			for (size_t j = 0; j < channelnames.size(); j++) 
			{
				it = std::find(_clients[i].getChannels().begin(), _clients[i].getChannels().end(), channelnames[j]);
				if (it != _clients[i].getChannels().end()) 
				{
					sendMsg(USER_ID(client->getNickname(), client->getUsername()) + " " + message + CRLF, _clients[i].getFd());
					break ;
				}
			}
		}
	}
}

string		Server::getActiveClients(void) 
{
	size_t i = 0;
	if (_clients.size() == 0)
		return "0";
	while (i < _clients.size())
		i++;

	std::ostringstream oss;
	oss << i;
	return oss.str();
}

string		Server::getActiveChannels(void) 
{
	size_t i = 0;
	if (_channels.size())
		return "0";
	while (i < _channels.size())
		i++;
	std::ostringstream oss;
	oss << i;
	return oss.str();
}

void	Server::setCreationTime() { _creationTime = getCurrentDataTime(); }

void	Server::setCreationTimeT() {  time_t now = time(0); _creationTimeT = now; }

string	Server::getCreationTime() { return _creationTime; }

time_t	Server::getCreationTimeT() { return _creationTimeT; }