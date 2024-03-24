#include "../inc/server.hpp"

bool Channel::isAdmin(Client client) {
	client_it it = admins.begin();
	while (it != admins.end()) {
		if (!((*it).getNickname()).compare(client.getNickname()))
			return true;
		it++;
	}
	return false;
}

bool Channel::isUser(Client client) {
	client_it it = users.begin();
	while (it != users.end()) {
		if (!((*it).getNickname()).compare(client.getNickname()))
			return true;
		it++;
	}
	return false;
}

Channel* Server::getChannel(std::string name) {
	channel_it it = channels.begin();
	while (it != channels.end()) {
		if (!(*it).getName().compare(name))
			return (&(*it));
		it++;
	}
	return NULL;
}

void Server::addChannel(std::string name) {
	Channel chan;
	chan.setName(name);
	channels.push_back(chan);
}

bool Server::channelExist(std::string name) {
	channel_it it = channels.begin();
	while (it != channels.end())
		if (!(*it).getName().compare(name))
			return true;
	return false;
}
