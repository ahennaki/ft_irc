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

	if (cmd.size() == 2)
		cmd.push_back("No reason set");

	std::vector<std::string> chans = ft_split(cmd[1], ',');
	for (size_t i = 0; i < chans.size(); i++) {
		if (chans[i][0] != '#' || !channelExist(chans[i]))
			replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), chans[i]));
		else {
			Channel* ch = getChannel(chans[i]);
			if (ch->isAdmin(*cli) || ch->isUser(*cli)) {
				ch->rmUser(*cli);
				client_it itc = clients.begin();
				while (itc != clients.end()) {
					if ((ch->isAdmin(*itc) || ch->isUser(*itc)) && (*itc).getFd() != fd)
						sendReplieToClient((*itc).getFd(), RPL_PARTCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), ch->getName(), ":" + cmd[2]));
					itc++;
				}
				replies(fd, RPL_PARTCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), ch->getName(), ":" + cmd[2]));
			}
			else
				replies(fd, ERR_NOTONCHANNEL(cli->getNickname(), ch->getName()));
		}
	}
}