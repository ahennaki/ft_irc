#include "server.hpp"

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
