#include "../inc/server.hpp"

void Server::rmClientFromChans(int fd) {
	Client* cli = getClient(fd);
	channel_it it = channels.begin();
	while (it != channels.end()) {
		Channel* ch = getChannel((*it).getName());
		if (ch->isAdmin(*cli) || ch->isUser(*cli))
			sendToAllUser(fd, ch, RPL_QUITCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd()));
		(*it).rmUser(*cli);
		it++;
	}
	replies(fd, RPL_QUITCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd()));
}

void Server::quitCmd(int fd) {
	std::cout << "Client \"" << fd << "\" Disconnected" << std::endl;
	rmClientFromChans(fd);
	rmClient(fd);
	close(fd);
}
