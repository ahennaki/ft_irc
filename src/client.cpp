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

void Server::registerClient(int fd) {
	if (getClient(fd)->getAuth() && !(getClient(fd)->getNickname()).empty() && !(getClient(fd)->getUsername()).empty()) {
		getClient(fd)->setRegistred(true);
		replies(fd, RPL_CONNECTED(getClient(fd)->getNickname()));
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
