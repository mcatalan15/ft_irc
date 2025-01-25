/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatalan <mcatalan@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 06:36:33 by mcatalan@st       #+#    #+#             */
/*   Updated: 2025/01/11 23:40:59 by jpaul-kr         ###   ########.fr       */
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
#define GREEN "\033[32m"
#define WHITE "\033[0m"

//Utils functions
std::vector<string>	splitMsg(string &str);
string				getCommandInUpper(const string &cmd);
std::vector<string>	splitCommand(string &cmd);

// Defines
#define CRLF "\r\n"
#define ERR_NOTREGISTERED(x) ("You are not registered: " + (x) + CRLF)
#define ERR_CMDNOTFOUND(nick, cmd) ("Command not found: " + (nick) + " -> " + (cmd) + CRLF)
#define ERR_UNKNOWNCOMMAND(nick, cmd) ("Unknown command: " + (nick) + " -> " + (cmd) + CRLF)

#endif
