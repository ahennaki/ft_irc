#include "../inc/server.hpp"
#include "../inc/utils.hpp"


std::vector<std::string> splitPrivmsg(std::string cmd) {
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

void Server::channelPrivmsg(int fd, std::string chan, std::string msg) {
	Channel* ch = getChannel(chan);
	Client* cli = getClient(fd);
	std::string cliNick = cli->getNickname();
	if (!ch) {
		replies(fd, ERR_NOSUCHCHANNEL(cliNick, chan)); return;
	}
	if (!ch->isAdmin(*cli) && !ch->isUser(*cli)) {
		replies(fd, ERR_USERNOTINCHANNEL(cliNick, chan)); return;
	}
	client_it it = clients.begin();
	while (it != clients.end()) {

	std::cout << " here" <<  RPL_PRIVMSGCHANNEL(cliNick, cli->getUsername(), cli->getIpadd(), chan, msg);
		if ((ch->isAdmin(*it) || ch->isUser(*it)) && it->getFd() != fd)
			sendReplieToClient((*it).getFd(), RPL_PRIVMSGCHANNEL(cliNick, cli->getUsername(), cli->getIpadd(), chan, msg));
		it++;
	}
}

void Server::clientPrivmsg(int fd, std::string nick, std::string msg) {
	Client* cli = getClient(fd);
	std::string cliNick = cli->getNickname();
	Client* target = getClient(nick);
	if (!target) {
		replies(fd, ERR_NOSUCHNICK(cliNick, nick)); return;
	}
	replies(target->getFd(), RPL_PRIVMSGUSER(cliNick, cli->getUsername(), cli->getIpadd(), nick, msg));
}

void Server::privmsgCmd(int fd, std::string cmd) {
	std::vector<std::string> privmsg(splitPrivmsg(cmd));
	Client* cli = getClient(fd);
	std::string cliNick = cli->getNickname();

	if (privmsg.size() < 3) {
		if (privmsg.size() < 2) {
			replies(fd, ERR_NORECIPIENT(cliNick)); return;
		}
		replies(fd, ERR_NOTEXTTOSEND(cliNick)); return;
	}
	std::vector<std::string> str(ft_split(privmsg[1], ','));
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i][0] == '#')
			channelPrivmsg(fd, str[i], privmsg[2]);
		else
			clientPrivmsg(fd, str[i], privmsg[2]);
	}
	
}
