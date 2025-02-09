#include "../include/Server.hpp"

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

//FONCTIONS
void Client::welcome()
{
    // NEED To IMPLEMENT
    if (_state != LOGIN || getNickname().empty() || getUsername().empty())
    {
        std::cout << "Waiting for registration..." << std::endl;
        return;
    }
    setState(REGISTERED);
    // NEED TO IMPLEMENT WELCOME MESSAGE
    std::cout << getNickname() << " is registered and ready to start !" << std::endl;
}
