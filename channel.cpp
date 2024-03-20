#include "server.hpp"

bool Channel::isAdmin(Client client) {
	admin_it it = admins.begin();
	while (it != admins.end()) {
		if (!((*it).getNickname()).compare(client.getNickname()))
			return 1;
		it++;
	}
	return 0;
}

void Server::addChannel(std::string name, std::string topic) {
	channel_it it = channels.begin();
	while (it != channels.end()) {
		if (!((*it).getName()).compare(name)) {
			(*it).setTopic(topic);
			break;
		}
		it++;
	}
	if (it == channels.end()) {
		Channel chan;
		chan.setName(name);
		chan.setTopic(topic);
		channels.push_back(chan);
	}
}
