#ifndef REPLIES_HPP
#define REPLIES_HPP

#define ERR_NOSUCHNICK(client, nick) (":irc.42.chat 401 " + client + " " + nick + " :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(client, chan) (":irc.42.chat 403 " + client + " " + chan + " :No such channel\r\n")
#define ERR_NORECIPIENT(client) (":irc.42.chat 411 " + client + " :No recipient given (PRIVMSG)\r\n")
#define ERR_NOTEXTTOSEND(client) (":irc.42.chat 412 " + client + " :No text to send\r\n")
#define ERR_UNKNOWNCOMMAND(client, command) (":irc.42.chat 421 " + client + " " + command + " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN(client) (":irc.42.chat 431 " + client + " :No nickname given\r\n")
#define ERR_ERRONEUSNICK(client) (":irc.42.chat 432 " + client + " :Erroneus nickname\r\n")
#define ERR_NICKINUSE(client) (":irc.42.chat 433 " + client + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(client, chan) (":irc.42.chat 441 " + client + " " + chan + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(client, chan) (":irc.42.chat 442 " + client + " " + chan + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(client, chan) (":irc.42.chat 443 " + client + " " + chan + " :is already on channel\r\n")
#define ERR_NOTREGISTERED(client) (":irc.42.chat 451 " + client + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(client) (":irc.42.chat 461 " + client + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTERED(client) (":irc.42.chat 462 " + client + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(client) (":irc.42.chat 464 " + client + " :Password incorrect\r\n")
#define ERR_CHANNELISFULL(client, chan) (":irc.42.chat 471 " + client + " " + chan + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(client, mode) (":irc.42.chat 472 " + client + " " + mode + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(client, chan) (":irc.42.chat 473 " + client + " " + chan + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, chan) (":irc.42.chat 475 " + client + " " + chan + " :Cannot join channel (+k)\r\n")
#define ERR_CHANOPRIVSNEEDED(chan) (":irc.42.chat 482 " + chan + " :You're not a channel operator\r\n")

#define RPL_JOINCHANNEL(client, user, ip, chan) (":" + client + "!~" + user + "@" + ip + " JOIN " + chan + "\r\n")
#define RPL_INVITEDCHANNEL(client, user, ip, chan) (":" + client + "!~" + user + "@" + ip + " INVITE " + chan + "\r\n")
#define RPL_KICKEDCHANNEL(client, user, ip, chan, nick) (":" + client + "!~" + user + "@" + ip + " KICK " + chan + " " + nick + "\r\n")
#define RPL_PARTCHANNEL(client, user, ip, chan, reason) (":" + client + "!~" + user + "@" + ip + " PART " + chan + " :" + reason + "\r\n")
#define RPL_PRIVMSGCHANNEL(client, user, ip, target, msg) (":" + client + "!~" + user + "@" + ip + " PRIVMSG " + target + " " + msg + "\r\n")
#define RPL_MODECHANNEL(client, user, ip, chan, mode, msg) (":" + client + "!~" + user + "@" + ip + " MODE " + chan + " " + mode + " " + msg + "\r\n")

#define RPL_WELCOME(client, user, ip) (":irc.42.chat 001 " + client + " :Welcome to the irc Network, " + client + "!~" + user + "@" + ip + "\r\n")
#define RPL_NOTOPIC(client, chan) (":irc.42.chat 331 " + client + " " + chan + " :No topic is set\r\n")
#define RPL_TOPIC(client, chan, topic) (":irc.42.chat 332 " + client + " " + chan + " :" + topic + "\r\n")
#define RPL_TOPICWHOTIME(client, chan, nick, setat) (":irc.42.chat 333 " + client + " " + chan + " " + nick + " " + setat + "\r\n")
#define RPL_INVITING(client, nick, chan) (":irc.42.chat 341 " + client + " " + nick + " " + chan + "\r\n")
#define RPL_CLIENTLIST(client, chan, clientslist) (":irc.42.chat 353 " + client + " = " + chan + " :" + clientslist + "\r\n")
#define RPL_ENDOFNAMES(client, chan) (":irc.42.chat 366 " + client + " " + chan + " :END of /NAMES list\r\n")

#endif
 