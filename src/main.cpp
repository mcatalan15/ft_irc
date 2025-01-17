/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatalan <mcatalan@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 06:34:29 by mcatalan@st       #+#    #+#             */
/*   Updated: 2025/01/11 20:57:42 by jpaul-kr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Irc.hpp"

int main(int argc, char **argv)
{
	if (argc != 3 || std::atoi(argv[1]) < 1 || std::atoi(argv[1]) > 65535) 
	{
		std::cerr << "Usage: ./irc <port> <password>" << std::endl;
		return (1);
	}
	Server	server(std::atoi(argv[1]), (string)argv[2]);

	server.client_process();
	return (0);
}
