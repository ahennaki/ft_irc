#include "../inc/server.hpp"

void Server::joinCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);
	if (!cli->getRegistred()) {
		replies(fd, ERR_NOTREGISTERED(cli->getNickname())); return;
	}
	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}
}
