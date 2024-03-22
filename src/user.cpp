 #include "../inc/server.hpp"

void Server::userCmd(int fd, std::vector<std::string> cmd) {
	if (cmd.size() < 5)
		replies(fd, ERR_NOTENOUGHPARAM(std::string("*")));
	else {
		getClient(fd)->setUsername(cmd[1]);
		registerClient(fd);
	}
}
