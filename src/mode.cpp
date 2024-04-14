#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Server::inviteOnly(int fd, char opr, std::string chan) {
	Channel* ch = getChannel(chan);
	Client* cli = getClient(fd);
	std::string clNick = cli->getNickname();
	std::string mode;
	if (opr == '+') {
		if (ch->i)
			return;
		mode = "+i";
		ch->i = true;
	}
	else if (opr == '-') {
		if (!ch->i)
			return;
		mode = "-i";
		ch->i = false;
	}
	sendToAllUser(fd, ch, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, ""));
	replies(fd, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, ""));
}

void Server::topicMode(int fd, char opr, std::string chan) {
	Channel* ch = getChannel(chan);
	Client* cli = getClient(fd);
	std::string clNick = cli->getNickname();
	std::string mode;
	if (opr == '+') {
		if (ch->t)
			return;
		mode = "+t";
		ch->t = true;
	}
	else if (opr == '-') {
		if (!ch->t)
			return;
		mode = "-t";
		ch->t = false;
	}
	sendToAllUser(fd, ch, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, ""));
	replies(fd, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, ""));
}

void Server::operatorMode(int fd, char opr, std::string chan, std::string nick) {
	Client* cli = getClient(nick);
	std::string clNick = cli->getNickname();
	Channel* ch = getChannel(chan);
	std::string mode;
	if (nick.empty()) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}
	if (!ch->isAdmin((*cli)) && !ch->isUser(*cli)) {
		replies(fd, ERR_USERNOTINCHANNEL(cli->getNickname(), ch->getName())); return;
	}
	if (opr == '+') {
		if (ch->isAdmin(*cli))
			return;
		ch->rmUser(*cli);
		ch->addAdmin(*cli);
		mode = "+o";
	}
	else if (opr == '-') {
		if (!ch->isAdmin(*cli))
			return;
		mode = "-o";
		ch->rmUser(*cli);
		ch->addUser(*cli);
	}
	sendToAllUser(fd, ch, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, nick));
	replies(fd, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, nick));
}

void Server::limitMode(int fd, char opr, std::string chan, std::string limit) {
	Channel* ch = getChannel(chan);
	Client* cli = getClient(fd);
	std::string clNick = cli->getNickname();
	std::string mode;
	if (opr == '+' && limit.empty()) {
		replies(fd, ERR_NEEDMOREPARAMS(getClient(fd)->getNickname())); return;
	}

	size_t lim = toInt(limit);
	if (opr == '+' && lim < 1) {
		replies(fd, ERR_UNKNOWNCOMMAND(getClient(fd)->getNickname(), "MODE +l" + limit)); return;
	}
	if (opr == '+') {
		if (ch->l)
			return;
		mode = "+l";
		ch->l = true;
		ch->limit = lim;
	}
	if (opr == '-') {
		if (!ch->l)
			return;
		mode = "-l";
		ch->l = false;
	}
	sendToAllUser(fd, ch, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, limit));
	replies(fd, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, limit));
}

void Server::keyMode(int fd, char opr, std::string chan, std::string key) {
	Channel* ch = getChannel(chan);
	Client* cli = getClient(fd);
	std::string clNick = cli->getNickname();
	std::string mode;
	// if (opr == '+') {
	// 	if (key.empty()) {
	// 		replies(fd, ERR_NEEDMOREPARAMS(getClient(fd)->getNickname())); return;
	// 	}
	// 	ch->k = true;
	// 	ch->setKey(key);
	// 	return;
	// }
	// ch->k = false;
	if (opr == '+') {
		if (key.empty()) {
			replies(fd, ERR_NEEDMOREPARAMS(getClient(fd)->getNickname())); return;
		}
		mode = "+k";
		ch->k = true;
		ch->setKey(key);
	}
	if (opr == '-') {
		if (!ch->k)
			return;
		mode = "-k";
		ch->k = false;
	}
	sendToAllUser(fd, ch, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, key));
	replies(fd, RPL_MODECHANNEL(clNick, cli->getUsername(), cli->getIpadd(), chan, mode, key));
}

void Server::modeExec(int fd, std::vector<std::string> cmd) {
	std::string opt = cmd[2];

	std::vector<std::string> args(cmd[2].size());
	for (size_t i = 3; i < cmd.size(); i++)
		args[i - 3] = cmd[i];

	for (size_t i = 1; i < opt.size(); i++) {
		if ((opt[0] != '-' && opt[0] != '+') || !isMode(opt[i])) {
			replies(fd, ERR_UNKNOWNMODE(getClient(fd)->getNickname(), opt)); return;
		}
	}
	
	size_t j = 0;
	for (size_t i = 1; i < opt.size(); i++) {
		if (opt[i] == 'i')
			inviteOnly(fd, opt[0], cmd[1]);
		else if (opt[i] == 't')
			topicMode(fd, opt[0], cmd[1]);
		else if (opt[i] == 'o')
			operatorMode(fd, opt[0], cmd[1], args[j++]);
		else if (opt[i] == 'l')
			limitMode(fd, opt[0], cmd[1], args[j++]);
		else if (opt[i] == 'k')
			keyMode(fd, opt[0], cmd[1], args[j++]);
	}
}

void Server::modeCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);
	if (cmd.size() < 3) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}

	Channel* chan = getChannel(cmd[1]);
	if (!chan) {
		replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), cmd[1])); return;
	}
	if (!chan->isUser(*cli) && !chan->isAdmin(*cli)) {
		replies(fd, ERR_USERNOTINCHANNEL(cli->getNickname(), chan->getName())); return;
	}
	if (chan->isUser(*cli)) {
		replies(fd, ERR_CHANOPRIVSNEEDED(chan->getName())); return;
	}
	modeExec(fd, cmd);
}
