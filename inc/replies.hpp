#ifndef REPLIES_HPP
#define REPLIES_HPP

#define ERR_NOSUCHNICK(client, nick) (": 401 " + client + " " + nick + " :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(client, chan) (": 403 " + client + " " + chan + " :No such channel\r\n")
#define ERR_UNKNOWNCOMMAND(client, command) (": 421 " + client + " " + command + " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN(client) (": 431 " + client + " :No nickname given\r\n")
#define ERR_ERRONEUSNICK(client) (": 432 " + client + " :Erroneus nickname\r\n")
#define ERR_NICKINUSE(client) (": 433 " + client + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(client, chan) (": 441 " + client + " " + chan + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(client, chan) (": 442 " + client + " " + chan + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(client, chan) (": 443 " + client + " " + chan + " :is already on channel\r\n")
#define ERR_NOTREGISTERED(client) (": 451 " + client + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(client) (": 461 " + client + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTERED(client) (": 462 " + client + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(client) (": 464 " + client + " :Password incorrect\r\n")
#define ERR_CHANNELISFULL(client, chan) (": 471 " + client + " " + chan + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(client, mode) (": 472 " + client + " " + mode + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(client, chan) (": 473 " + client + " " + chan + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, chan) (": 475 " + client + " " + chan + " :Cannot join channel (+k)\r\n")
#define ERR_CHANOPRIVSNEEDED(chan) (": 482 " + chan + " :You're not a channel operator\r\n")

#define RPL_CONNECTED(client) (": 001 " + client + " : Welcome to the IRC server\r\n")
#define RPL_JOINCHANNEL(client, user, ipaddress, chan) (":" + client + "!" + user + "@" + ipaddress + " JOIN " + chan + "\r\n")
#define RPL_INVITEDCHANNEL(client, user, ipaddress, chan) (":" + client + "!" + user + "@" + ipaddress + " INVITE " + chan + "\r\n")
#define RPL_KICKEDCHANNEL(client, user, ipaddress, chan, nick) (":" + client + "!" + user + "@" + ipaddress + " KICK " + chan + " " + nick + "\r\n")
#define RPL_NOTOPIC(client, chan) (": 331 " + client + " " + chan + " :No topic is set\r\n")
#define RPL_TOPIC(client, chan, topic) (": 332 " + client + " " + chan + " :" + topic + "\r\n")
#define RPL_TOPICWHOTIME(client, chan, nick, setat) (": 333 " + client + " " + chan + " " + nick + " " + setat + "\r\n")
#define RPL_INVITING(client, nick, chan) (": 341 " + client + " " + nick + " " + chan + "\r\n")
#define RPL_CLIENTLIST(client, chan, clientslist) (": 353 " + client + " = " + chan + " :" + clientslist + "\r\n")
#define RPL_ENDOFNAMES(client, chan) (": 366 " + client + " " + chan + " :END of /NAMES list\r\n")

#endif
