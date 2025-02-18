#ifndef CMDREGISTRATION_HPP
# define CMDREGISTRATION_HPP

/*------------------------------------*/
/*-------CONNECTION REGISTRATION------*/
/*------------------------------------*/

/*------------PASSEWORD MESSAGES------*/
/*  The password supplied must match the one contained in
  the C/N lines (for servers) or I lines (for clients).
  It is possible to send multiple PASS commands before
  registering but only the last one sent is used for
  verification and it may not be changed once registered.
  Numeric Replies:*/
//ERR_NEEDMOREPARAMS
//ERR_ALREADYREGISTRED

/*------------NICK MESSAGES-----------*/
/* If a NICK message arrives at a server which already knows
   about an identical nickname for another client, a nickname
   collision occurs. As a result of a nickname collision, all
   instances of the nickname are removed from the server's
   database, and a KILL command is issued to remove the
   nickname from all other server's database. If the NICK
   message causing the collision was a nickname change,
   then the original (old) nick must be removed as well.

   If the server recieves an identical NICK from a client
   which is directly connected, it may issue an
   ERR_NICKCOLLISION to the local client, drop the NICK
   command, and not generate any kills.*/
//ERR_NONICKNAMEGIVEN
//ERR_NICKNAMEINUSE
//ERR_ERRONEUSNICKNAME
//ERR_NICKCOLLISION

/*-----------USER MESSAGES-----------*/
//ERR_NEEDMOREPARAMS
//ERR_ALREADYREGISTRED

/*-----------SERVER MESSAGES---------*/
//ERR_ALREADYREGISTRED
// maybe we don't need this one

/*----------OPER MESSAGES------------*/
/*If the client sending the OPER command supplies the
  correct password for the given user, the server then
  informs the rest of the network of the new operator by
  issuing a "MODE +o" for the clients nickname.
  The OPER message is client-server only.*/
//ERR
//ERR_NEEDMOREPARAMS
//ERR_NOOPERHOST
//ERR_ERR_PASSWMISMATCH
//RPL
//RPL_YOUREOPER

/*-----------NAMES MESSAGES------------*/
/*Command: NAMES
   Parameters: [<channel>{,<channel>}]

   By using the NAMES command, a user can list all nicknames that are
   visible to them on any channel that they can see.  Channel names
   which they can see are those which aren't private (+p) or secret (+s)
   or those which they are actually on.  The <channel> parameter
   specifies which channel(s) to return information about if valid.
   There is no error reply for bad channel names.

   If no <channel> parameter is given, a list of all channels and their
   occupants is returned.  At the end of this list, a list of users who
   are visible but either not on any channel or not on a visible channel
   are listed as being on `channel' "*".*/
//RPL_NAMREPLY
//RPL_ENDOFNAMES

/*------------LIST MESSAGE---------------*/
/*Command: LIST
   Parameters: [<channel>{,<channel>} [<server>]]

   The list message is used to list channels and their topics.  If  the
   <channel>  parameter  is  used,  only  the  status  of  that  channel
   is displayed.  Private  channels  are  listed  (without  their
   topics)  as channel "Prv" unless the client generating the query is
   actually on that channel.  Likewise, secret channels are not listed
   at  all  unless  the client is a member of the channel in question.*/
//ERR
//ERR_NOSUCHSERVER
//RPL
//RPL_LIST
//RPL_LISTSTART
//RPL_LISTEND

/*--------------INVITE MESSAGE-------------*/
/*The INVITE message is used to invite users to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  To invite a user to a channel which is invite only (MODE
   +i), the client sending the invite must be recognised as being a
   channel operator on the given channel.*/
//ERR
//ERR_NEEDMOREPARAMS
//ERR_NOTONCHANNEL
//ERR_CHANOPRIVSNEEDED
//ERR_NOSUCHNICK
//ERR_USERONCHANNEL
//RPL
//RPL_INVITING
//RPL_AWAY

/*---------------KICK COMMAND---------------*/
//ERR_NEEDMOREPARAMS
//ERR_BADCHANMASK
//ERR_NOTONCHANNEL
//ERR_NOSUCHCHANNEL
//ERR_CHANOPRIVNEEDED
   #endif
