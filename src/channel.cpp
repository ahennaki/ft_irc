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
	return channel;
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

bool Channel::isInvited(Client client) {
	client_it it = invited.begin();
	while (it != invited.end()) {
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

void Server::addClientToChan(int fd, std::string name, std::string key) {
	Channel* ch = getChannel(name);
	Client* cli = getClient(fd);
	if (ch->i && !ch->isInvited(*cli)) {
		replies(fd, ERR_INVITEONLYCHAN(cli->getNickname(), ch->getName())); return;
	}
	if (ch->l && ch->limit <= ch->userNbr()) {
		replies(fd, ERR_CHANNELISFULL(cli->getNickname(), ch->getName())); return;
	}
	if (ch->k && ch->getKey().compare(key)) {
		replies(fd, ERR_BADCHANNELKEY(cli->getNickname(), name)); return;
	}
	ch->addUser(*cli);
	replies(fd, RPL_JOINCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), name));
	replies(fd, RPL_CLIENTLIST(cli->getNickname(), name, ch->getClientList()));
	replies(fd, RPL_ENDOFNAMES(cli->getNickname(), name));
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

size_t Channel::userNbr() {
	client_it it = admins.begin();
	size_t nb = 0;
	while (it != admins.end()) {
		nb++;
		it++;
	}
	client_it itu = users.begin();
	while (itu != users.end()) {
		nb++;
		itu++;
	}
	return nb;
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
