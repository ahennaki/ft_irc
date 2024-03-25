#include "../inc/server.hpp"

bool Channel::isAdmin(Client client) {
	if (client.getAdmin())
		return true;
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
		if (chan->isUser(*cli)) {
			replies(fd, ERR_USERONCHANNEL(cli->getNickname(), name)); return;
		}
		else {
			addClientToChan(fd, name);
			replies(fd, RPL_JOINMSG(cli->getNickname(), cli->getUsername(), cli->getIpadd(), name));
			replies(fd, RPL_NAMREPLY(cli->getNickname(), name, chan->getClientList()));
			replies(fd, RPL_ENDOFNAMES(cli->getNickname(), name));
			return;
		}
	}
	Channel chan;
	chan.setName(name);
	chan.setKey(key);
	cli->setAdmin(true);
	chan.addUser(*cli);
	channels.push_back(chan);
	replies(fd, RPL_JOINMSG(cli->getNickname(), cli->getUsername(), cli->getIpadd(), name));
	replies(fd, RPL_NAMREPLY(cli->getNickname(), name, ("@" + cli->getNickname())));
	replies(fd, RPL_ENDOFNAMES(cli->getNickname(), name));
}

void Server::addClientToChan(int fd, std::string name) {
	getChannel(name)->addUser(*getClient(fd));
}

std::string Channel::getClientList() {
	client_it it = users.begin();
	std::string clist;
	while (it != users.end()) {
		if ((*it).getAdmin())
			clist += ("@" + (*it).getNickname());
		else
			clist += (*it).getNickname();
		clist += " ";
		it++;
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
