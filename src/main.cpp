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
		std::cerr << e.what() << std::endl;
	}
	std::cout << "\nServer closed" << std::endl;
	return (0);
}
