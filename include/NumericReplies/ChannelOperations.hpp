#ifndef CHANNELOPERATIONS_HPP
# define CHANNELOPERATIONS_HPP

/*------------JOIN MESSSAGE-----------*/
//ERR
//ERR_NEEDMOREPARAMS
//ERR_INVITEONLYCHAN
//ERR_CHANNELISFULL
//ERR_NOSUCHCHANNEL
//ERR_BANNEDFROMCHAN
//ERR_BADCHANNELKEY
//ERR_BADCHANMASK
//ERR_TOMANYCHANNELS
//RPL
//RPL_TOPIC

/*--------------PART MESSAGE-----------*/
//ERR_NEEDMOREPARAMS
//ERR_NOTONCHANNEL
//ERR_NOSUCHCHANNEL

/*--------------MODE MESSAGE-----------*/
//i - invite-only channel flag;
//t - topic settable by channel operator only flag;
//k - set a channel key (password);
//o - give/take channel operator privileges;
//l - set the user limit to channel;
/* When using the 'o' and 'b' options, a restriction on a
   total of three per mode command has been imposed.
   That is, any combination of 'o' and */

/*-------------TOPIC MESSAGE-----------*/
//ERR
//ERR_NEEDMOREPARAMS
//ERR_CHANOPRIVSNEEDED
//ERR_NOTONCHANNEL
//RPL
//RPL_NOTOPIC
//RPL_TOPIC

#endif
