#include "../include/Server.hpp"
#include <cstddef>

// Default constructor
Client::Client() : _fd(-1), _isOper(false), _state(HANDSHAKE) {}

// Constructor
Client::Client(int client_fd) :_fd(client_fd), _isOper(false), _state(HANDSHAKE) {
	_nickname.clear();
    _username.clear();
    _realname.clear();
    _hostname.clear();
}

// Destructor
Client::~Client() {}

// Copy constructor
Client::Client(Client const &src) { *this = src; }

// Assignment operator overload "="
Client &Client::operator=(Client const &src) {
	if (this != &src) {
		_fd = src._fd;
		_nickname = src._nickname;
		_username = src._username;
		_realname = src._realname;
		_hostname = src._hostname;
		_isOper = src._isOper;
		_state = src._state;
	}
	return (*this);
}

//GETTERS
int Client::getFd() { return (_fd); }

string Client::getNickname() const { return (_nickname); }

string Client::getUsername() const { return (_username); }

string Client::getRealname() const { return (_realname); }

string Client::getHostname() const { return (_hostname); }

bool Client::getIsOper() const { return (_isOper); }

State Client::getState() const { return (_state); }

string Client::getMsg() const { return (_msg); }

string &Client::getMsgRef() { return (_msg); }

// To build the prefix in IRC format: ":nickname!username@hostname"
string Client::getPrefix() const {
    string prefix = ":" + getNickname();
    if (!getUsername().empty())
        prefix += "!" + getUsername();
    if (!getHostname().empty())
        prefix += "@" + getHostname();
    return prefix;
}

//SETTERS
void Client::setNickname(const std::string& nickname) { _nickname = nickname; }

void Client::setUsername(const std::string& username) { _username = username; }

void Client::setRealname(const std::string& realname) { _realname = realname; }

void Client::setHostname(const std::string& hostname) { _hostname = hostname; }

void Client::setState(State newState) { _state = newState; }

void Client::setIsOper(bool isOper) { _isOper = isOper; }

void Client::setMsg(const std::string& msg) { _msg = msg; }

//FONCTIONS
void Client::welcome(Client &Client, int fd) {
	string userId = USER_ID(Client.getNickname(), Client.getUsername());
	sendMsg(RPL_WELCOME(Client.getNickname(), userId), fd);
	sendMsg(RPL_YOURHOST(Client.getUsername(), SERVER_NAME, SERVER_VERSION), fd);
	//sendMsg(RPL_CREATED(Client.getUsername(), _timer), fd); FALTA TIMER!!!!!
	sendMsg(RPL_MYINFO(Client.getUsername(), SERVER_NAME, SERVER_VERSION, USER_MODES, CHANNEL_MODES, CHANNEL_MODES_WITH_PARAM), fd);
	string supportedTokens = "NICKLEN=9";
	sendMsg(RPL_ISSUPPORT(Client.getUsername(), supportedTokens), fd);
	sendMsg(RPL_MOTDSTART(Client.getUsername(), SERVER_NAME), fd);
	sendMsg(RPL_MOTD(Client.getUsername(),  
		" ______  _______    ______  \n"  
		"|      \\|       \\  /      \\ \n"  
		" \\$$$$$$| $$$$$$$\\|  $$$$$$\\\n"  
		"  | $$  | $$__| $$| $$   \\$$\n"  
		"  | $$  | $$    $$| $$      \n"  
		"  | $$  | $$$$$$$\\| $$   __ \n"  
		" _| $$_ | $$  | $$| $$__/  \\\n"  
		"|   $$ \\| $$  | $$ \\$$    $$\n"  
		" \\$$$$$$ \\$$   \\$$  \\$$$$$$  \n"), fd);
	sendMsg(RPL_ENDOFMOTD(Client.getUsername()), fd);

    // NEED To IMPLEMENT
    //if (_state != LOGIN || getNickname().empty() || getUsername().empty()) {
    //    std::cout << "Waiting for registration..." << std::endl;
    //    return;
    //}
    //setState(REGISTERED);
    // NEED TO IMPLEMENT WELCOME MESSAGE
    //std::cout << getNickname() << " is registered and ready to start !" << std::endl;
}
/* NOT used NOW
void	Client::clearSpecMsg() {
	size_t	lp = this->_msg.find_last_of("\r\n");
	
	if (lp != this->_msg.size())
		this->_msg.erase(lp, this->_msg.size());
	else
		this->_msg.clear();
}*/

void	Client::cleanBuff() {
	this->_msg.clear();
}

void	Client::appendToMsg(const string &msg) { _msg += msg; }
