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
	Client* target = getClient(nick);
	Client* cli = getClient(fd);
	Channel* ch = getChannel(chan);
	std::string mode;

	if (nick.empty()) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}
	if (!target) {
		replies(fd, ERR_NOSUCHNICK(cli->getNickname(), nick)); return;
	}
	if (!ch->isAdmin((*target)) && !ch->isUser(*target)) {
		replies(fd, ERR_USERNOTINCHANNEL(target->getNickname(), ch->getName())); return;
	}
	if (opr == '+') {
		if (!ch->isAdmin(*target)) {
			ch->rmUser(*target);
			ch->addAdmin(*target);
		}
		mode = "+o";
	}
	else if (opr == '-') {
		if (ch->isAdmin(*target)) {
			ch->rmUser(*target);
			ch->addUser(*target);
		}
		mode = "-o";
	}
	sendToAllUser(fd, ch, RPL_MODECHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), chan, mode, nick));
	replies(fd, RPL_MODECHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), chan, mode, nick));
}

void Server::limitMode(int fd, char opr, std::string chan, std::string limit) {
	Channel* ch = getChannel(chan);
	Client* cli = getClient(fd);
	std::string clNick = cli->getNickname();
	std::string mode;
	if (opr == '+' && limit.empty()) {
		replies(fd, ERR_NEEDMOREPARAMS(getClient(fd)->getNickname())); return;
	}

	int lim = toInt(limit);
	if (opr == '+' && lim < 1) {
		replies(fd, ERR_UNKNOWNCOMMAND(getClient(fd)->getNickname(), "MODE +l" + limit)); return;
	}
	if (opr == '+') {
		if (ch->l && ch->limit == lim)
			return;
		ch->l = true;
		ch->limit = lim;
		mode = "+l";
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

	if (opr == '+') {
		if (key.empty()) {
			replies(fd, ERR_NEEDMOREPARAMS(getClient(fd)->getNickname())); return;
		}
		if (ch->k) {
			ch->setKey(key); return;
		}
		ch->k = true;
		ch->setKey(key);
		mode = "+k";
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
	std::vector<std::string> args;

	for (size_t i = 3; i < cmd.size(); i++)
		args.push_back(cmd[i]);
	while (args.size() < opt.size())
		args.push_back("");	

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

std::string getModes(Channel ch) {
	std::string modes;

	if (ch.i || ch.t || ch.k || ch.l) {
		modes = "+";
		if (ch.i)
			modes += "i";
		if (ch.t)
			modes += "t";
		if (ch.k)
			modes += "k";
		if (ch.l)
			modes += "l";
	}
	if (ch.k)
		modes += " " + ch.getKey();
	if (ch.l)
		modes += " " + toString(ch.limit);
	std::cout << "limit " << toString(ch.limit) << std::endl;
	return modes;
}

void Server::modeCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);
	if (cmd.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}

	Channel* chan = getChannel(cmd[1]);
	if (!chan) {
		replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), cmd[1])); return;
	}
	if (cmd.size() == 2) {
		std::string mode = getModes(*chan);
		replies(fd, RPL_CHANNELMODEIS(cli->getNickname(), chan->getName(), mode));
		replies(fd, RPL_CREATIONTIME(cli->getNickname(), chan->getName(), chan->chanDate));
		return;
	}
	if (!chan->isUser(*cli) && !chan->isAdmin(*cli)) {
		replies(fd, ERR_USERNOTINCHANNEL(cli->getNickname(), chan->getName())); return;
	}
	if (chan->isUser(*cli)) {
		replies(fd, ERR_CHANOPRIVSNEEDED(chan->getName())); return;
	}
	modeExec(fd, cmd);
}
