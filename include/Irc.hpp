/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatalan@student.42barcelona.com <mcata    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 06:36:33 by mcatalan@st       #+#    #+#             */
/*   Updated: 2025/02/12 13:10:45 by jpaul-kr         ###   ########.fr       */
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
std::vector<string>	splitUserCmd(string &cmd);
void				printVecStr(std::vector<string> cmd);
string				addHostname();
void				sendMsg(string msg, int fd);
bool				nickChecker(string cmd);
string				getCurrentDataTime();
std::vector<string>	joinDivisor(string cmd);
bool validChannel(string& channelName, int fd);

// Defines
#define NICK_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_[]\\{}|-"
#define USER_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_[]\\{}|-*. "
#define CHANNEL_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_[]\\{}|-* #@"
#define MAX_CLIENTS 25
#define MAX_CONNECTIONS 5
#define MAX_CHANNELS 5
#define CRLF "\r\n"
#define SERVER_NAME "FT_IRC"
#define SERVER_VERSION "1.0"
#define USER_MODES		"" // que mierdas es esto
#define CHANNEL_MODES	"itkol" // y esto otro?
#define CHANNEL_MODES_WITH_PARAM "kol" //copiada historica

#define USER_ID(nick, user) (":" + (nick) + "!" + (user) + "@localhost")
#define ERR_NOTREGISTERED(x) ("You are not registered: " + (x) + CRLF)
#define ERR_CMDNOTFOUND(nick, cmd) ("Command not found: " + (nick) + " -> " + (cmd) + CRLF)
#define ERR_UNKNOWNCOMMAND(nick, cmd) (":localhost 421 " + nick + " " + cmd + " :Unknown command" + CRLF)

//WELCOME msg (RPL_WELCOME)
#define RPL_WELCOME(nick, userId) (":localhost 001 " + (nick) + " :Welcome to the Internet Relay Chat Network, " + (userId) + CRLF)
#define RPL_YOURHOST(client, servername, version) (":localhost 002 " + (client) + " :Your host is " + (servername) + ", running version Release Candidate(RC)-" + (version) + CRLF)
#define RPL_CREATED(client, datatime) (":localhost 003 " + (client) + " :This server was created " + (datatime) + CRLF)
#define RPL_MYINFO(client, servername, version, availableUserModes, availableChannelModes, channelModesWithParam)\
	(":localhost 004 " + client + " " + servername + " " + version + " " + availableUserModes + " " + availableChannelModes + " " + channelModesWithParam"" + CRLF)
#define RPL_ISSUPPORT(client, supportedTokens) (":localhost 005 " + client + " " + supportedTokens + " :are supported by this server" + CRLF)

//MOTD msg
#define RPL_MOTDSTART(client, servername)(":localhost 375 " + client + " :- " + servername + " Messsage of the day -" + CRLF)
#define RPL_MOTD(client, line)(":localhost 372 " + client + " :" + line + CRLF)
#define RPL_ENDOFMOTD(client)(":localhost 376 " + client + " :End of /MOTD command." + CRLF)

//INFO cmd
#define RPL_INFO(client, string)(":localhost 371 " + (client) + " :" + (string) + CRLF)
#define RPL_ENDOFINFO(client)(":localhost 374 " + (client) + " :End of INFO list" + CRLF)

//PASS cmd
#define ERR_PASSWDMISMATCH(client)(":localhost 464 " + (client) + " : Password incorrect" + CRLF)

//NICK cmd
#define ERR_NONICKNAMEGIVEN()(string(":localhost 431 * :No nickname given") + CRLF)
#define ERR_ERRONEUSNICKNAME(nick)(":localhost 432 * " + (nick) + " :Erroneus nickname" + CRLF)
#define ERR_NICKNAMEINUSE(nick)(":localhost 433 * " + (nick) + " :Nickname is already in use" + CRLF)

//USER cmd
#define ERR_ERRONEUSUSERNAME(user)(":localhost 432 * " + (user) + " :Erroneus nickname" + CRLF)
#define ERR_USERNAMEINUSE(user)(":localhost 433 * " + (user) + " :Nickname is already in use" + CRLF)

// JOIN CMD RPL
#define RPL_CONNECT(nick, user, channel)(USER_ID(nick, user) + " JOIN :" + channel + CRLF)
#define RPL_NAMREPLY(nick, channel, msg)(":localhost 353 " + (nick) + " = " + (channel) + (msg) + CRLF)
#define RPL_TOPIC(client, channel, topic)(":localhost 332 "+ (channel) + " :" + (topic) + CRLF)
#define RPL_TOPICWHOTIME(client, channel, nick, setat)(":localhost 333 " + (client) + " " + (channel) +" " + (nick) + " " + (setat) + CRLF)
#define ERR_NOSUCHCHANNEL(client, channel)(":localhost 403 " + (client) + " " + (channel) + " :No such channel" + CRLF)
#define ERR_TOOMANYCHANNELS(client, channel)(":localhost 405 " + (client)+ " " + (channel) + " :You have joined too many channels" + CRLF)
#define ERR_TOOMANYCHANNELSCREATED(client, channel)(":localhost 405 " + (client)+ " " + (channel) + " :too many channels have been created" + CRLF)
#define ERR_BADCHANNELKEY(client, channel)(":localhost 475 " + (client) +" " + (channel) + " :Cannot join channel (+k)" + CRLF)
#define ERR_BANNEDFROMCHAN(client, channel)(":localhost 474 " + (client) + " " + (channel) + " :Cannot join channel (+b)"+ CRLF)
#define	ERR_CHANNELISFULL(client, channel)(":localhost 471 " + (client) + " " + (channel) + " :Cannot join channel (+l)" + CRLF)
#define ERR_INVITEONLYCHAN(client, channel)(":localhost 473 " +(client) + " " + (channel) + " :Cannot join channel (+i)" + CRLF)
#define ERR_BADCHANMASK(channel)(":localhost 476 " + (channel) + " :Bad Channel Mask" + CRLF)
#define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + (client) + " " + (channel) + " :End of /NAMES list" + CRLF)

// MODE MD
#define RPL_CHANNELMODEIS(client, channel, modestring, modeargs)(":localhost 324 " + (client) + " " + (channel) + " " + (modestring) + " " + (modeargs) + CRLF)
#define RPL_CREATIONTIME(client, channel, ctime)(":localhost 329 " + (client) + " " + (channel) + " " + (ctime) + CRLF)
#define ERR_CHANOPRIVSNEEDED(client, channel)(":localhost 482 " + (client) + " " + (channel) + " :You're not channel operator" + CRLF)
//#define ERR_UNKNOWNMODE(client, modechar)(":localhost 472 " + (client) + " " + (modechar) + " :is unknown mode char to me" + CRLF)
#define ERR_UMODEUNKOWNFLAG(client)(":localhost 501 " + (client) + " :Unknown MODE flag" + CRLF)
#define ERR_KEYSET(channel)(":localhost 467 " + (channel) + " :Channel key already set" + CRLF)
#define ERR_INVALIDMODEPARAM(client, channel, modechar, parameter, description)(" :localhost 696 " + (client) + " " + (channel) + " " + (modechar) + " " + (parameter) + " :" + (description) + CRLF)
#define ERR_INVALIDKEY(client, channel)(":localhost 525 " + (client) + " " + (channel) + " :Key is not well-formed" + CRLF)

// PART cmd
#define  ERR_NOTONCHANNEL(client, channel)(":localhost 442 " + (client) + " " + (channel) + " :You're not on that channel" + CRLF)
#define  ERR_NOSUCHCHANNEL(client, channel)(":localhost 403 " + (client) + " " + (channel) + " :No such channel" + CRLF)

//KICK cmd
#define ERR_USERNOTINCHANNEL(client, nick, channel)(":localhost 441 " + (client) + " " + (nick) + " " + (channel) + " :They aren't on that channel" + CRLF)
#define  ERR_CHANOPRIVSNEEDED(client, channel)(":localhost 482 " + (client) + " " + (channel) + " :You're not channel operator" + CRLF)
#define  ERR_CANNOTKICK(client, nick, channel)(":localhost 482 " + (client) + " " + (nick) + " " + (channel) + " :can't kick" + CRLF)

//PRIVMSG cmd
#define  ERR_NOSUCHCHANNELORCLIENT(client, arg)(":localhost 401 " + (client) + " " + (arg) + " :No such channel or client" + CRLF)

// INVITE cmd
#define ERR_USERONCHANNEL(client, nick, channel)(":localhost 443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel" + CRLF)
#define RPL_ENDOFINVITELIST(client)(": localhost 337 " + (client) + " :End of /INVITE list" + CRLF)
#define RPL_INVITING(client, nick, channel)(":localhost 341 " + (client) + " " + (nick) + " " + (channel) + CRLF)

// GENERAL ERR
#define ERR_NEEDMOREPARAMS(client, cmd)(":localhost 461 " + (client) + " " + (cmd) + " :Not enought parameters" + CRLF)
#define ERR_ALREADYREGISTERED(nick)(":localhost 462 " + (nick) + " :You may not register" + CRLF)
#define ERR_NOSUCHNICK(client, nick)(":localhost 401 " + (client) + " " + (nick) + " :No such nick" + CRLF)

//TOPIC cmd
#define RPL_NOTOPIC(client, channel)(":localhost 331: "+ (client) + " " + (channel) + " :No topic is set" + CRLF)
//:localhost 464 marc :Password incorrect
//nickname
//:localhost 433 marc mcs :Nickname is already in use
//user
//USER * * :marc catalan sanchez
//:localhost 461 marc USER :Not enough parameters


#endif
