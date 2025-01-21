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
# define IRC_HPP



// System includes
# include <string>
# include <map>
# include <iostream>
# include <cstdlib>
# include <sys/poll.h>	// -> poll()
# include <sys/epoll.h>	// -> epoll()
# include <sys/socket.h>// -> socket(), bind(), listen(), accept(), connect()
# include <netinet/in.h>// -> sockaddr_in()
# include <errno.h>		// -> errno
# include <unistd.h>	
# include <sstream>
# include <fcntl.h>		// -> fcntl()		
# include <ctime>
# include <cstring>
# include <arpa/inet.h> // -> inet_ntoa()
# include <csignal>		// -> signal()
# include <algorithm>
# include <vector>		// -> vector

# define MAX_CLIENTS 1024

using std::string;

// Own includes
# include "Server.hpp"
//# include "ErrorLog.hpp"
#endif
