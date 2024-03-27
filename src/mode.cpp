#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Server::inviteOnly(char opr, std::string chan) {
	Channel* ch = getChannel(chan);
	if (opr == '+') {
		if (ch->i)
			return;
		ch->i = true;
	}
	else if (opr == '-') {
		if (!ch->i)
			return;
		ch->i = false;
	}
}

void Server::topicMode(char opr, std::string chan) {
	Channel* ch = getChannel(chan);
	if (opr == '+') {
		if (ch->t)
			return;
		ch->t = true;
	}
	else if (opr == '-') {
		if (!ch->t)
			return;
		ch->t = false;
	}
}

void Server::operatorMode(int fd, char opr, std::string chan, std::string nick) {
	Client* cli = getClient(nick);
	Channel* ch = getChannel(chan);
	if (!ch->isAdmin((*cli)) && !ch->isUser(*cli)) {
		replies(fd, ERR_TARGETNOTONCHANNEL(cli->getNickname(), ch->getName())); return;
	}
	if (opr == '+') {
		if (ch->isAdmin(*cli))
			return;
		ch->rmUser(*cli);
		ch->addAdmin(*cli);
	}
	else if (opr == '-') {
		if (!ch->isAdmin(*cli))
			return;
		ch->rmUser(*cli);
		ch->addUser(*cli);
	}
}

void Server::modeExec(int fd, std::vector<std::string> cmd) {
	std::string opt = cmd[2];

	std::vector<std::string> args(cmd[2].size());
	for (size_t i = 3; i < cmd.size(); i++)
		args[i - 3] = cmd[i];

	size_t j = 0;
	char opr = '/0';
	for (size_t i = 0; i < opt.size(); i++) {
		if (opt[i] == '+' || opt[i] == '-')
			opr = opt[i];
		else {
			if (opt[i] == 'i')
				inviteOnly(opr, cmd[1]);
			else if (opt[i] = 't')
				topicMode(opr, cmd[1]);
			else if (opt[i] == 'o')
				operatorMode(fd, opr, cmd[1], args[j++]);
		}
	}
	
}

void Server::modeCmd(int fd, std::vector<std::string> cmd) {
	Client* cli = getClient(fd);
	if (cmd.size() < 3) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}
	if (cmd[1][0] != '#' || !channelExist(cmd[1])) {
		replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), cmd[1])); return;
	}

	Channel* chan = getChannel(cmd[1]);
	if (!chan->isUser(*cli) && !chan->isAdmin(*cli)) {
		replies(fd, ERR_CLINOTONCHANNEL(cli->getNickname(), chan->getName())); return;
	}
	if (chan->isUser(*cli)) {
		replies(fd, ERR_NOTOPERATOR(chan->getName())); return;
	}
	if (cmd[2].size() == 1) {
		if (cmd[2].find_first_not_of("+-iotkl") == std::string::npos) {
			replies(fd, ERR_NOTRECOGNISEDOPT(cli->getNickname(), cmd[1], cmd[2])); return;
		}
		return;
	}
	modeExec(fd, cmd);
}
