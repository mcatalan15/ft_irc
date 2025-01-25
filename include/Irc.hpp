/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatalan@student.42barcelona.com <mcata    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 06:36:33 by mcatalan@st       #+#    #+#             */
/*   Updated: 2025/01/25 15:51:06 by mcatalan@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

// System includes
#include <string>
#include <map>
#include <iostream>
#include <cstdlib>
#include <sys/poll.h>	// -> poll()
#include <sys/socket.h>// -> socket(), bind(), listen(), accept(), connect()
#include <netinet/in.h>// -> sockaddr_in()
#include <errno.h>		// -> errno
#include <unistd.h>	
#include <sstream>
#include <fcntl.h>		// -> fcntl()		
#include <ctime>
#include <string>
#include <arpa/inet.h> // -> inet_ntoa()
#include <csignal>		// -> signal()
#include <algorithm>
#include <vector>		// -> vector
#include <cstddef>		// -> size_t

#define MAX_CLIENTS 1024

using std::string;

// Own includes
//# include "ErrorLog.hpp"

// Colors
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define BOLD_BLACK   "\033[1;30m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_GREEN   "\033[1;32m"
#define BOLD_YELLOW  "\033[1;33m"
#define BOLD_BLUE    "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN    "\033[1;36m"
#define BOLD_WHITE   "\033[1;37m"

#define BACKGROUND_BLACK   "\033[40m"
#define BACKGROUND_RED     "\033[41m"
#define BACKGROUND_GREEN   "\033[42m"
#define BACKGROUND_YELLOW  "\033[43m"
#define BACKGROUND_BLUE    "\033[44m"
#define BACKGROUND_MAGENTA "\033[45m"
#define BACKGROUND_CYAN    "\033[46m"
#define BACKGROUND_WHITE   "\033[47m"

// Reset color
#define RESET "\033[0m"
//Utils functions
std::vector<string>	splitMsg(string &str);
string				getCommandInUpper(const string &cmd);
std::vector<string>	splitCommand(string &cmd);

// Defines
#define CRLF "\r\n"
#define ERR_NOTREGISTERED(x) ("You are not registered: " + (x) + CRLF)
#define ERR_CMDNOTFOUND(nick, cmd) ("Command not found: " + (nick) + " -> " + (cmd) + CRLF)
#define ERR_UNKNOWNCOMMAND(nick, cmd) ("Unknown command: " + (nick) + " -> " + (cmd) + CRLF)
//password
//:localhost 464 marc :Password incorrect
//nickname
//:localhost 433 marc mcs :Nickname is already in use
//user
//USER * * :marc catalan sanchez
//:localhost 461 marc USER :Not enough parameters


#endif
