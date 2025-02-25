#include "../../include/Server.hpp"

bool Server::isOnChan(string& cmd, int fd) {
	(void)fd;
	std::vector<string> operatorList = findChannel(cmd)->getOperators();
	for (size_t i = 0; i < operatorList.size(); i++) {
		if (findChannel(cmd)->isOperator(operatorList[i]))
			return true;
		else
			return false;
	}
	return false;
}

void	Server::topicDisplay(string& cmd, int fd) {
	for (size_t i = 0; i < _channels.size(); i++) {
		if (cmd == _channels[i].getName()) {
			if (isOnChan(cmd, fd)) {
				if (_channels[i].getTopic().empty())
					return(sendMsg(RPL_NOTOPIC(getClient(fd)->getNickname(), cmd) , fd));
				else {
					sendMsg(RPL_TOPIC(getClient(fd)->getNickname(), cmd, _channels[i].getTopic()), fd);
					return (sendMsg(RPL_TOPICWHOTIME(getClient(fd)->getNickname(), cmd, getClient(fd)->getNickname(),_channels[i].getCreationTime()), fd));
				}
			} else
				return(sendMsg(ERR_NOTONCHANNEL(getClient(fd)->getNickname(), cmd), fd));
		} else 
			return(sendMsg(ERR_NOSUCHCHANNEL(getClient(fd)->getNickname(), cmd), fd));
	}
}

void	Server::topicSetter(std::vector<string>& cmd, int fd) {
	if (findChannel(cmd[1])->isModeSet(TOPIC_RESTRICTED)) {
		if (findChannel(cmd[1])->isOperator(getClient(fd)->getUsername())) {
			findChannel(cmd[1])->setTopic(cmd[2]);
			string msg = "TOPIC ";
			msg.append(cmd[1]);
			msg.append(" ");
			msg.append(cmd[2]);
			sendMsgToChannel(msg, findChannel(cmd[1]), fd);
		} else
			sendMsg(ERR_CHANOPRIVSNEEDED(getClient(fd)->getNickname(), cmd[1]), fd);
	} else {
		findChannel(cmd[1])->setTopic(cmd[2]);
		string msg = "TOPIC ";
		msg.append(cmd[1]);
		msg.append(" ");
		msg.append(cmd[2]);
		sendMsgToChannel(msg, findChannel(cmd[1]), fd);
	}
}
