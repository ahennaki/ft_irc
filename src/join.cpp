#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Server::addChannel(int fd, std::string name, std::string key) {
	Client* cli = getClient(fd);
	if (channelExist(name)) {
		Channel* chan = getChannel(name);
		if (chan->isUser(*cli) || chan->isAdmin(*cli)) {
			replies(fd, ERR_USERONCHANNEL(cli->getNickname(), name)); return;
		}
		else {
			addClientToChan(fd, name, key); return;
		}
	}
	Channel ch = createChannel(fd, name, key);
	channels.push_back(ch);
	replies(fd, RPL_JOINCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), name));
	replies(fd, RPL_CLIENTLIST(cli->getNickname(), name, ("@" + cli->getNickname())));
	replies(fd, RPL_ENDOFNAMES(cli->getNickname(), name));
}

void Server::joinCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);

	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}
	std::vector<std::string> chans = ft_split(cmd[1], ',');
	std::vector<std::string> keys(chans.size());
	if (!cmd[2].empty())
		keys = ft_split(cmd[2], ',');

	for (size_t i = 0; i < chans.size(); i++) {
		// if (chans[i][0] != '0')
		// 	part();
		if (chans[i][0] != '#')
			replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), chans[i]));
		else
			addChannel(fd, chans[i], keys[i]);
	}
	// std::cout << "exit join" << std::endl;
	
}
