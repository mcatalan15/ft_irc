#include "../include/Server.hpp"

bool Server::isOnChan(string& cmd, int fd) {
	(void)fd;
	std::vector<string> operatorList = findChannel(cmd)->getOperators();
	for (size_t i = 0; i < operatorList.size(); i++) {
		if (findChannel(cmd)->isOperator(operatorList[i])) { // GETOPERATORS func
			// Topic to a channel where is memmber
			std::cout << "is on channel" << std::endl;
			return true;
		}
		else {
			// Topic to a chanell where is NOT member
			std::cout << "isn't on channel" << std::endl;
		}
	}
	return false;
}

void	Server::topicDisplay(string& cmd, int fd) {
	for (size_t i = 0; i < _channels.size(); i++) {
		if (cmd == _channels[i].getName()) {
			// name is correct
			std::cout << "channel exists" << std::endl;
			if (isOnChan(cmd, fd)) {
				if (_channels[i].getTopic().empty()) {
					//RPL_NOTOPIC
					return(sendMsg(RPL_NOTOPIC(getClient(fd)->getNickname(), cmd) , fd));
				} else {
					//RPL_TOPIC
					sendMsg(RPL_TOPIC(getClient(fd)->getNickname(), cmd, _channels[i].getTopic()), fd);
					return (sendMsg(RPL_TOPICWHOTIME(getClient(fd)->getNickname(), cmd, getClient(fd)->getNickname(),_channels[i].getCreationTime()), fd));
				}
			} else {
				// the client is not in the channel
				return(sendMsg(ERR_NOTONCHANNEL(getClient(fd)->getNickname(), cmd), fd));
			}
		} else {
			// channel no exist
			return(sendMsg(ERR_NOSUCHCHANNEL(getClient(fd)->getNickname(), cmd), fd));
		}
	}
}

void	Server::topicSetter(std::vector<string>& cmd, int fd) {
//mirar si el topic esta ON o no para saber si operador o todos
	std::cout << "topic + 2 size" << std::endl;
	if (findChannel(cmd[1])->isModeSet(TOPIC_RESTRICTED)) {
		std::cout << "TOPIC restringido" << std::endl;
		// is restricted to the operators
		if (findChannel(cmd[1])->isOperator(getClient(fd)->getUsername())) {
			//is operator
			std::cout << "es operador" << cmd[1] << " " << getClient(fd) << std::endl;
			findChannel(cmd[1])->setTopic(cmd[2]);
		} else {
			//no es operador, a cagarrrrr
			std::cout << "no es operador" << cmd[1] << " " << getClient(fd) << std::endl;
		}
	} else {
		// channel not restricted
		std::cout << "Channel no esta restrignido" << std::endl;
		findChannel(cmd[1])->setTopic(cmd[2]);
		string msg = " TOPIC ";
		msg.append(cmd[1]);
		msg.append(" ");
		msg.append(cmd[2]);
		sendMsgToChannel(msg, findChannel(cmd[1]), fd);
	}
}
