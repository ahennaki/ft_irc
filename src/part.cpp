#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Server::partChannels(int fd) {
	Client* cli = getClient(fd);
	channel_it it = channels.begin();
	while (it != channels.end()) {
		if ((*it).isUser(*cli) || (*it).isAdmin(*cli)) {
			Channel* ch = getChannel((*it).getName());
			ch->rmUser(*cli);
			replies(fd, RPL_PARTCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), ch->getName(), ""));
			sendToAllUser(fd, ch, RPL_PARTCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), ch->getName(), ""));
		}
		it++;
	}
}

void Server::partCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);

	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}

	std::string reason;
	if (cmd.size() == 2)
		reason = "";
	else {
		if (cmd[2][0] == ':') {
			if (cmd[2][1] == ':')
				cmd[2].erase(0, 1);
			for (size_t i = 2; i < cmd.size(); i++)
				reason += cmd[i] + " ";
		}
		else
			reason = ":" + cmd[2];	
	}

	std::vector<std::string> chans = ft_split(cmd[1], ',');
	for (size_t i = 0; i < chans.size(); i++) {
		if (chans[i][0] != '#' || !channelExist(chans[i]))
			replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), chans[i]));
		else {
			Channel* ch = getChannel(chans[i]);
			if (ch->isAdmin(*cli) || ch->isUser(*cli)) {
				sendToAllUser(fd, ch, RPL_PARTCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), ch->getName(), reason));
				ch->rmUser(*cli);
				replies(fd, RPL_PARTCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), ch->getName(), reason));
			}
			else
				replies(fd, ERR_NOTONCHANNEL(cli->getNickname(), ch->getName()));
		}
	}
}