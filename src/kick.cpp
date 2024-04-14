#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Server::kickCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);
	if (cmd.size() < 3) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}
	Channel* ch = getChannel(cmd[1]);
	if (!ch) {
		replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), cmd[1])); return;
	}
	if (!ch->isAdmin(*cli) && !ch->isUser(*cli)) {
		replies(fd, ERR_NOTONCHANNEL(cli->getNickname(), cmd[1])); return;
	}
	if (!ch->isAdmin(*cli)) {
		replies(fd, ERR_CHANOPRIVSNEEDED(cmd[1])); return;
	}
	std::vector<std::string> users(ft_split(cmd[2], ','));
	str_it it = users.begin();
	while (it != users.end()) {
		Client* usr = getClient(*it);
		if (!usr) {
			replies(fd, ERR_NOSUCHNICK(cli->getNickname(), *it)); return;
		}
		if (!ch->isAdmin(*usr) && !ch->isUser(*usr)) {
			replies(fd, ERR_USERNOTINCHANNEL(cli->getNickname(), cmd[1])); return;
		}
		sendToAllUser(fd, ch, RPL_KICKEDCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), cmd[1], usr->getNickname()));
		ch->rmUser(*usr);
		replies(fd, RPL_KICKEDCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), cmd[1], usr->getNickname()));
		it++;
	}
}
