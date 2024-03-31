#include "../inc/server.hpp"

void Server::inviteCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);
	if (cmd.size() < 3) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}

	Client* target = getClient(cmd[1]);
	Channel* ch = getChannel(cmd[2]);
	if (!target) {
		replies(fd, ERR_NOSUCHNICK(cmd[1], cmd[2])); return;
	}
	if (!ch) {
		replies(fd, ERR_NOSUCHCHANNEL(cmd[1], cmd[2])); return;
	}
	if (!ch->isUser(*cli) && !ch->isAdmin(*cli)) {
		replies(fd, ERR_NOTONCHANNEL(cli->getNickname(), cmd[2])); return;
	}
	if (ch->i && !ch->isAdmin(*cli)) {
		replies(fd, ERR_CHANOPRIVSNEEDED(cmd[2])); return;
	}
	if (ch->isAdmin(*target) || ch->isUser(*target)) {
		replies(fd, ERR_USERONCHANNEL(target->getNickname(), cmd[2])); return;
	}
	ch->addInvited(*target);
	replies(fd, RPL_INVITING(cli->getNickname(), cmd[1], cmd[2]));
	replies(target->getFd(), RPL_INVITEDCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), cmd[2]));
}
