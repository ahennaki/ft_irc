#include "../inc/server.hpp"

void Server::passCmd(int fd, std::vector<std::string> cmd) {
	if (cmd.size() < 2)
		replies(fd, ERR_NOTENOUGHPARAM(std::string("*")));
	else if (cmd.size() == 2) {
		if (!cmd[1].compare(password)) {
			getClient(fd)->setAuth(true);
			registerClient(fd);
			return;
		}
	}
	replies(fd, ERR_INCORPASS(std::string("*")));
}
