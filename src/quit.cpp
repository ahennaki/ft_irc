#include "../inc/server.hpp"

void Server::rmClientFromChans(int fd) {
	Client* cli = getClient(fd);
	channel_it it = channels.begin();
	while (it != channels.end()) {
		(*it).rmUser(*cli);
		it++;
	}
}

void Server::quitCmd(int fd) {
	std::cout << "Client \"" << fd << "\" Disconnected" << std::endl;
	rmClientFromChans(fd);
	rmClient(fd);
	close(fd);
}
