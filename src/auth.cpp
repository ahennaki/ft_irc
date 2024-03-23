#include "../inc/server.hpp"

void Server::passCmd(int fd, std::vector<std::string> cmd) {
	if (cmd.size() < 2)
		replies(fd, ERR_NOTENOUGHPARAM(std::string("*")));
	else if (cmd.size() == 2) {
		if (!cmd[1].compare(password)) {
			getClient(fd)->setAuth(true);
			return;
		}
		else
			replies(fd, ERR_INCORPASS(std::string("*")));
	}
	else
		replies(fd, ERR_INCORPASS(std::string("*")));
}

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
	else if (cmd.size() >= 2) {
		if (!isNickValid(cmd[1]))
			replies(fd, ERR_ERRONEUSNICK(cmd[1]));
		else if (isNickUsed(cmd[1]))
			replies(fd, ERR_NICKINUSE(cmd[1]));
		else {
			if (!getClient(fd)->getAuth())
				replies(fd, ERR_NOTREGISTERED(getClient(fd)->getNickname()));
			getClient(fd)->setNickname(cmd[1]);
			if (!getClient(fd)->getRegistred())
				registerClient(fd);
		}
	}
}

void Server::userCmd(int fd, std::vector<std::string> cmd) {
	if (cmd.size() < 5)
		replies(fd, ERR_NOTENOUGHPARAM(std::string("*")));
	else {
		if (!getClient(fd)->getAuth())
			replies(fd, ERR_NOTREGISTERED(getClient(fd)->getNickname()));
		getClient(fd)->setUsername(cmd[1]);
		if (!getClient(fd)->getRegistred())
			registerClient(fd);
	}
}
