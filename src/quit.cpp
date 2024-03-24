#include "../inc/server.hpp"

void Server::quitCmd(int fd) {
	std::cout << "Client \"" << fd << "\" Disconnected" << std::endl;
	rmClient(fd);
	close(fd);
}
