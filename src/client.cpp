#include "../inc/server.hpp"

void Server::rmClient(int fd) {
	pfd_it p_it = pfd.begin();
	while (p_it != pfd.end()) {
		if ((*p_it).fd == fd) {
			pfd.erase(p_it);
			break;
		}
		p_it++;
	}

	client_it it = clients.begin();
	while (it != clients.end()) {
		if ((*it).getFd() == fd) {
			clients.erase(it);
			break;
		}
		it++;
	}
}

Client* Server::getClient(int fd) {
	client_it it = clients.begin();
	while (it != clients.end()) {
		if ((*it).getFd() == fd)
			return (&(*it));
		it++;
	}
	return NULL;
}

Client* Server::getClient(std::string nick) {
	client_it it = clients.begin();
	while (it != clients.end()) {
		if ((*it).getNickname() == nick)
			return (&(*it));
		it++;
	}
	return NULL;
}

void Server::registerClient(int fd) {
	Client* cli = getClient(fd);
	if (cli->getAuth() && !(cli->getNickname()).empty() && !(cli->getUsername()).empty()) {
		cli->setRegistred(true);
		replies(fd, RPL_WELCOME(cli->getNickname(), cli->getUsername(), cli->getIpadd()));
		replies(fd, RPL_YOURHOST(cli->getNickname()));
		replies(fd, RPL_CREATED(cli->getNickname(), date));
	}
}

void Server::addClient(int fd, std::string nickName, std::string userName, std::string realName) {
	client_it it = clients.begin();
	while (it != clients.end()) {
		if ((*it).getFd() == fd) {
			(*it).setNickname(nickName);
			(*it).setRealname(realName);
			(*it).setUsername(userName);
		}
		it++;
	}
}
