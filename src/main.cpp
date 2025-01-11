/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatalan <mcatalan@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 06:34:29 by mcatalan@st       #+#    #+#             */
/*   Updated: 2025/01/11 10:29:14 by mcatalan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"



int main(int argc, char **argv)
{
	if (argc != 3 || std::atoi(argv[1]) < 1 || std::atoi(argv[1]) > 65535) {
		std::cerr << "Usage: ./irc <port> <password>" << std::endl;
		return (1);
	}
	// IRC irc(std::atoi(argv[1]));
	// irc.run();
	return (0);
}
