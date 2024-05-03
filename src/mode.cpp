#include "../inc/server.hpp"
#include "../inc/utils.hpp"

std::string Server::inviteOnly(char opr, std::string chan) {
	Channel* ch = getChannel(chan);
	std::string mode;

	if (opr == '+') {
		if (ch->i)
			return mode;
		mode = "+i";
		ch->i = true;
	}
	else if (opr == '-') {
		if (!ch->i)
			return mode;
		mode = "-i";
		ch->i = false;
	}
	return mode;
}

std::string Server::topicMode(char opr, std::string chan) {
	Channel* ch = getChannel(chan);
	std::string mode;

	if (opr == '+') {
		if (ch->t)
			return mode;
		mode = "+t";
		ch->t = true;
	}
	else if (opr == '-') {
		if (!ch->t)
			return mode;
		mode = "-t";
		ch->t = false;
	}
	return mode;
}

std::string Server::operatorMode(int fd, char opr, std::string chan, std::string nick) {
	Client* target = getClient(nick);
	Client* cli = getClient(fd);
	Channel* ch = getChannel(chan);
	std::string mode;

	if (nick.empty()) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return mode;
	}
	if (!target) {
		replies(fd, ERR_NOSUCHNICK(cli->getNickname(), nick)); return mode;
	}
	if (!ch->isAdmin((*target)) && !ch->isUser(*target)) {
		replies(fd, ERR_USERNOTINCHANNEL(target->getNickname(), ch->getName())); return mode;
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
	return mode;
}

std::string Server::limitMode(int fd, char opr, std::string chan, std::string limit) {
	Channel* ch = getChannel(chan);
	std::string mode;

	if (opr == '+' && limit.empty()) {
		replies(fd, ERR_NEEDMOREPARAMS(getClient(fd)->getNickname())); return mode;
	}

	long long lim = toInt(limit);
	if (opr == '+' && (lim < 1 || lim > INT_MAX)) {
		return mode;
	}
	if (opr == '+') {
		if (ch->l && ch->limit == lim)
			return mode;
		ch->l = true;
		ch->limit = lim;
		mode = "+l";
	}
	if (opr == '-') {
		if (!ch->l)
			return mode;
		mode = "-l";
		ch->l = false;
	}
	return mode;
}

std::string Server::keyMode(int fd, char opr, std::string chan, std::string key) {
	Channel* ch = getChannel(chan);
	std::string mode;

	if (opr == '+') {
		if (key.empty()) {
			replies(fd, ERR_NEEDMOREPARAMS(getClient(fd)->getNickname())); return mode;
		}
		if (ch->k) {
			ch->setKey(key); return mode;
		}
		ch->k = true;
		ch->setKey(key);
		mode = "+k";
	}
	if (opr == '-') {
		if (!ch->k)
			return mode;
		mode = "-k";
		ch->k = false;
	}
	return mode;
}

void Server::modeExec(int fd, std::vector<std::string> cmd) {
	std::string opt = cmd[2];
	std::vector<std::string> args;
	std::string mode;
	Client* cli = getClient(fd);

	for (size_t i = 3; i < cmd.size(); i++)
		args.push_back(cmd[i]);
	while (args.size() < opt.size())
		args.push_back("");

	for (size_t i = 0; i < opt.size(); i++) {
		if (opt[i] != '-' && opt[i] != '+' && !isMode(opt[i])) {
			replies(fd, ERR_UNKNOWNMODE(getClient(fd)->getNickname(), opt)); return;
		}
	}
	char oper = '+';
	size_t j = 0;
	for (size_t i = 0; i < opt.size(); i++) {
		if (opt[i] == '+' || opt[i] == '-')
			oper = opt[i];
		else if (opt[i] == 'i')
			mode += inviteOnly(oper, cmd[1]);
		else if (opt[i] == 't')
			mode += topicMode(oper, cmd[1]);
		else if (opt[i] == 'o')
			mode += operatorMode(fd, oper, cmd[1], args[j++]);
		else if (opt[i] == 'l') {
			std::string str = limitMode(fd, oper, cmd[1], args[j]);
			if (str.empty())
				args[j] = "";
			mode += str;
			j++;
		}
		else if (opt[i] == 'k') {
			std::string str = keyMode(fd, oper, cmd[1], args[j]);
			if (str.empty())
				args[j] = "";
			mode += str;
			j++;
		}
	}
	if (!mode.empty()) {
		std::string arg;
		for (size_t i = 0; i < args.size(); i++)
			arg += args[i] + " ";
		Channel* ch = getChannel(cmd[1]);
		sendToAllUser(fd, ch, RPL_MODECHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), cmd[1], mode, arg));
		replies(fd, RPL_MODECHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), cmd[1], mode, arg));
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
