#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Server::passCmd(int fd, std::vector<std::string> cmd) {
	Client *cli = getClient(fd);
	// if (cmd[1][0] == ':')
	// 	cmd[1].erase(0, 1);
	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(std::string("*"))); return;
	}
	else {
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
	std::string old = cli->getNickname();
	std::string str = "*";

	if (cmd.size() < 2)
		replies(fd, ERR_NONICKNAMEGIVEN(cli->getNickname()));
	else {
		if (!isNickValid(cmd[1])) {
			if (cmd[1][0] == ':' && cmd[1][1] == ':') {
					replies(fd, ERR_ERRONEUSNICK(str)); return;
			}
			if (cmd[1][0] == ':') {
					replies(fd, ERR_ERRONEUSNICK(str)); return;
			}
			replies(fd, ERR_ERRONEUSNICK(cmd[1])); return;
		}
		if (isNickUsed(cmd[1], fd)) {
			replies(fd, ERR_NICKINUSE(cmd[1])); return;
		}
		if (!cli->getAuth()){
			replies(fd, ERR_NOTREGISTERED(cli->getNickname())); return;
		}
		if (old.empty())
			cli->setNickname(cmd[1]);
		else {
			if (!old.compare(cmd[1]))
				return;
			cli->setNickname(cmd[1]);
			if (cli->getRegistred()) {
				channel_it it = channels.begin();
				while (it != channels.end()) {
					// std::cout << "user: " << (*it).isUser(*cli) << std::endl;
					if ((*it).isAdmin(*cli) || (*it).isUser(*cli)) {
						Channel* ch = getChannel((*it).getName());
						// std::cout << "checked" << std::endl;
						sendToAllUser(fd, ch, RPL_CHANGENICK(old, cli->getUsername(), cli->getIpadd(), cmd[1]));
					}
					it++;
				}
				replies(fd, RPL_CHANGENICK(old, cli->getUsername(), cli->getIpadd(), cmd[1]));
			}
		}
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
