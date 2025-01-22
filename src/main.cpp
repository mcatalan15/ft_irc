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

#include "../include/Server.hpp"

int main(int argc, char **argv) {
	if (argc != 3 || std::atoi(argv[1]) < 1024 || std::atoi(argv[1]) > 49151) {
		std::cerr << "Usage: ./irc <port> <password>" << std::endl;
		return (1);
	}
	Server	server(std::atoi(argv[1]), (string)argv[2]);
	try {
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		signal(SIGPIPE, SIG_IGN);
		server.client_process();
	} catch (const std::exception &e) {
		//server.closeFds(); -> uncomment + finish...
		std::cerr << e.what() << std::endl;
	}
	std::cout << "\nServer closed" << std::endl;
	return (0);
}
