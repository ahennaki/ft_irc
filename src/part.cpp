#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Server::partCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);

	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}

	std::vector<std::string> chans = ft_split(cmd[1], ',');
	for (size_t i = 0; i < chans.size(); i++) {
		if (chans[i][0] != '#' || !channelExist(chans[i]))
			replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), chans[i]));
		else {
			Channel* ch = getChannel(chans[i]);
			if (ch->isAdmin(*cli) || ch->isUser(*cli))
				ch->rmUser(*cli);
		}
	}
}