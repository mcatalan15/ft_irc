#include "../include/Irc.hpp"

/*
Client::Client(int server_fd)
{
    fd = fd;
    _nickname.clear();
    _username.clear();
    _realname.clear();
    _hostname.clear();
    _isOper = false;
    _state = HANDSHAKE;
}
*/
Client::~Client() {}

/*
//GETTERS
int Client::getFd()
{
    return (fd);
}

std::string Client::getNickname() const
{
    return (_nickname);
}

std::string Client::getUsername() const
{
    return (_username);
}

std::string Client::getRealname() const
{
    return (_realname);
}

std::string Client::getHostname() const
{
    return (_hostname);
}

bool Client::getIsOper() const
{
    return (_isOper);
}

State Client::getState() const
{
    return (_state);
}

// To build the prefix in IRC format: ":nickname!username@hostname"
std::string Client::getPrefix() const
{
    std::string prefix = ":" + getNickname();
    if (!getUsername().empty())
        prefix += "!" + getUsername();
    if (!getHostname().empty());
        prefix += "@" + getHostname();
    return (prefix);
}

//SETTERS
void Client::setNickname(const std::string& nickname)
{
    _nickname = nickname;
}

void Client::setUsername(const std::string& username)
{
    _username = username;
}

void Client::setRealname(const std::string& realname)
{
    _realname = realname;
}

void Client::setHostname(const std::string& hostname)
{
    _hostname = hostname;
}

void Client::setState(State newState)
{
    _state = newState;
}

void Client::setIsOper(bool isOper)
{
    _isOper = isOper;
}

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
*/
