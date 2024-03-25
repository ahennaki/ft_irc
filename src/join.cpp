#include "../inc/server.hpp"

std::vector<std::string> splitJoin(std::string cmd, char delimiter) {
	std::vector<std::string> tokens;
	std::string str;
	size_t pos = 0;

	while (pos < cmd.size()) {
		if (cmd[pos] != delimiter)
			str += cmd[pos];
		else {
			tokens.push_back(str);
			str.clear();
		}
		++pos;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}

void Server::joinCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);

	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}
	std::vector<std::string> chans = splitJoin(cmd[1], ',');
	std::vector<std::string> keys(chans.size());
	if (!cmd[2].empty())
		keys = splitJoin(cmd[2], ',');

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



void Server::partCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);

	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}

	std::vector<std::string> chans = splitJoin(cmd[1], ',');
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
