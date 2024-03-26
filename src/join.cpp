#include "../inc/server.hpp"
#include "../inc/utils.hpp"

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
