#include "../inc/server.hpp"

Channel Server::createChannel(int fd, std::string name, std::string key) {
	Channel channel;
	channel.i = false;
	channel.t = false;
	channel.k = false;
	channel.l = false;
	channel.limit = 0;
	channel.setName(name);
	channel.setKey(key);
	channel.addAdmin(*getClient(fd));
}

bool Channel::isAdmin(Client client) {
	client_it it = admins.begin();
	while (it != admins.end()) {
		if ((*it).getFd() == client.getFd())
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

void Server::addClientToChan(int fd, std::string name) {
	Channel* ch = getChannel(name);
	Client* cli = getClient(fd);
	if (ch->i && !cli->getInvited()) {
		replies(fd, ERR_CANTJOINCHANNEL(cli->getNickname(), ch->getName())); return;
	}
	ch->addUser(*cli);
}

std::string Channel::getClientList() {
	client_it it = admins.begin();
	std::string clist;
	while (it != admins.end()) {
		clist += ("@" + (*it).getNickname());
		clist += " ";
		it++;
	}
	client_it itu = users.begin();
	while (itu != users.end()) {
		clist += (*itu).getNickname();
		clist += " ";
		itu++;
	}
	return clist;
}

bool Server::channelExist(std::string name) {
	channel_it it = channels.begin();
	while (it != channels.end()) {
		if (!(*it).getName().compare(name))
			return true;
		it++;
	}
	return false;
}

bool Server::clientExist(int fd, std::string name) {
	if (getChannel(name)->isUser(*getClient(fd)))
		return true;
	return false;
}

void Channel::rmUser(Client client) {
	client_it it = admins.begin();
	while (it != admins.end()) {
		if ((*it).getFd() == client.getFd()) {
			admins.erase(it); return;
		}
		it++;
	}

	client_it itu = users.begin();
	while (itu != users.end()) {
		if ((*itu).getFd() == client.getFd()) {
			users.erase(itu); return;
		}
		itu++;
	}
}
