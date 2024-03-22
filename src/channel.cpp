#include "../inc/server.hpp"

bool Channel::isAdmin(Client client) {
	admin_it it = admins.begin();
	while (it != admins.end()) {
		if (!((*it).getNickname()).compare(client.getNickname()))
			return true;
		it++;
	}
	return false;
}

bool Channel::isUser(Client client) {
	admin_it it = users.begin();
	while (it != users.end()) {
		if (!((*it).getNickname()).compare(client.getNickname()))
			return true;
		it++;
	}
	return false;
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
