#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Server::passCmd(int fd, std::vector<std::string> cmd) {
	Client *cli = getClient(fd);
	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(std::string("*"))); return;
	}
	else if (cmd.size() == 2) {
		if (cli->getAuth()) {
			replies(fd, ERR_ALREADYREGISTERED(cli->getNickname())); return;
		}
		if (!cmd[1].compare(password)) {
			cli->setAuth(true); return;
		}
	}
	replies(fd, ERR_PASSWDMISMATCH(std::string("*")));
}

void Server::nickCmd(int fd, std::vector<std::string> cmd) {
	Client *cli = getClient(fd);
	if (cmd.size() < 2)
		replies(fd, ERR_NONICKNAME(cli->getNickname()));
	else {
		if (!isNickValid(cmd[1])) {
			replies(fd, ERR_ERRONEUSNICK(cmd[1])); return;
		}
		if (isNickUsed(cmd[1])) {
			replies(fd, ERR_NICKINUSE(cmd[1])); return;
		}
		if (!cli->getAuth()){
			replies(fd, ERR_NOTREGISTERED(cli->getNickname())); return;
		}
		cli->setNickname(cmd[1]);
		if (!cli->getRegistred()) {
			registerClient(fd); return;
		}
	}
}

void Server::userCmd(int fd, std::vector<std::string> cmd) {
	Client *cli = getClient(fd);
	if (cmd.size() < 5)
		replies(fd, ERR_NEEDMOREPARAMS(std::string("*")));
	else {
		if (!cli->getAuth()){
			replies(fd, ERR_NOTREGISTERED(cli->getNickname())); return;
		}
		if (!cli->getUsername().empty()) {
			replies(fd, ERR_ALREADYREGISTERED(cli->getNickname())); return;
		}
		else
			cli->setUsername(cmd[1]);
		if (!cli->getRegistred()) {
			registerClient(fd); return;
		}
	}
}
