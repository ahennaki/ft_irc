#include "../inc/server.hpp"

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

void Server::addChannel(int fd, std::string name, std::string key) {
	Client* cli = getClient(fd);
	if (channelExist(name)) {
		Channel* chan = getChannel(name);
		if (chan->isUser(*cli) || chan->isAdmin(*cli)) {
			replies(fd, ERR_USERONCHANNEL(cli->getNickname(), name)); return;
		}
		else {
			addClientToChan(fd, name);
			replies(fd, RPL_JOINCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), name));
			replies(fd, RPL_CLIENTLIST(cli->getNickname(), name, chan->getClientList()));
			replies(fd, RPL_ENDOFNAMES(cli->getNickname(), name));
			return;
		}
	}
	Channel chan;
	chan.setName(name);
	chan.setKey(key);
	chan.addAdmin(*cli);
	channels.push_back(chan);
	replies(fd, RPL_JOINCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), name));
	replies(fd, RPL_CLIENTLIST(cli->getNickname(), name, ("@" + cli->getNickname())));
	replies(fd, RPL_ENDOFNAMES(cli->getNickname(), name));
}

void Server::addClientToChan(int fd, std::string name) {
	getChannel(name)->addUser(*getClient(fd));
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
