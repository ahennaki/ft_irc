#include "../inc/server.hpp"

bool Server::isNickUsed(std::string nick) {
	client_it it = clients.begin();
	while (it != clients.end()) {
		if (!((*it).getNickname()).compare(nick))
			return true;
		it++;
	}
	return false;
}

bool isNickValid(std::string nick) {
	if (nick[0] == '#' || nick[0] == ':' || nick[0] == '&')
		return false;
	for (size_t i = 0; i < nick.size(); i++)
		if (!std::isalnum(nick[i]))
			return false;
	return true;
}

void Server::nickCmd(int fd, std::vector<std::string> cmd) {
	if (cmd.size() < 2)
		replies(fd, ERR_NOTENOUGHPARAM(std::string("*")));
	else if (cmd.size() == 2) {
		if (!isNickValid(cmd[1]))
			replies(fd, ERR_ERRONEUSNICK(cmd[1]));
		else if (isNickUsed(cmd[1]))
			replies(fd, ERR_NICKINUSE(cmd[1]));
		else {
			getClient(fd)->setNickname(cmd[1]);
			registerClient(fd);
		}
	}
}
