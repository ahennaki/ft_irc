#include "../inc/server.hpp"
#include "../inc/utils.hpp"

void Topic::setName(std::string topic) {
	std::vector<std::string> str(splitCmd(topic));
	if (topic[0] == ':') {
		topic.erase(0, 1);
		this->topic = topic;
	}
	else
		this->topic = str[0];
}

std::vector<std::string> splitTopic(std::string cmd) {
	std::vector<std::string> tokens;
	std::string str;
	size_t pos = 0;

	while (pos < cmd.size()) {
		if (cmd[pos] != ' ')
			str += cmd[pos];
		else {
			tokens.push_back(str);
			str.clear();
		}
		pos++;
		if (tokens.size() == 2)
			break;
	}
	if (tokens.size() == 2)
		str = cmd.substr(pos);
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}

void Server::topicCmd(int fd, std::string cmd) {
	std::vector<std::string> topic(splitTopic(cmd));
	Client* cli = getClient(fd);
	if (topic.size() < 2) {
		replies(fd, ERR_NEEDMOREPARAMS(cli->getNickname())); return;
	}
	Channel* ch = getChannel(topic[1]);
	if (!ch) {
		replies(fd, ERR_NOSUCHCHANNEL(cli->getNickname(), topic[1])); return;
	}
	Topic* top =  ch->getTopic();
	if (topic.size() == 2) {
		if (top->getName().empty()) {
			replies(fd, RPL_NOTOPIC(cli->getNickname(), topic[1])); return;
		}
		replies(fd, RPL_TOPIC(cli->getNickname(), topic[1], top->getName()));
		replies(fd, RPL_TOPICWHOTIME(cli->getNickname(), topic[1],top->getNick(), top->getTime()));
		return;
	}
	if (!ch->isAdmin(*cli) && !ch->isUser(*cli)) {
		replies(fd, ERR_NOTONCHANNEL(cli->getNickname(), topic[1])); return;
	}
	if (ch->t) {
		if (!ch->isAdmin(*cli)) {
			replies(fd, ERR_CHANOPRIVSNEEDED(topic[1])); return;
		}
	}
	top->setName(topic[2]);
	top->setNick(cli->getNickname());
	top->setTime(getCurrentTime());
	sendToAllUser(fd, ch, RPL_TOPICCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), topic[1], top->getName()));
	replies(fd, RPL_TOPICCHANNEL(cli->getNickname(), cli->getUsername(), cli->getIpadd(), topic[1], top->getName()));
}
