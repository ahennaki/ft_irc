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
