#ifndef REPLIES_HPP
#define REPLIES_HPP

#define ERR_NOSUCHCHANNEL(nick, chan) (": 403 " + nick + " " + chan + " :No such channel\r\n")
#define ERR_UNKNOWNCOMMAND(nick, command) (": 421 " + nick + " " + command + " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN(nick) (": 431 " + nick + " :No nickname given\r\n")
#define ERR_ERRONEUSNICK(nick) (": 432 " + nick + " :Erroneus nickname\r\n")
#define ERR_NICKINUSE(nick) (": 433 " + nick + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(nick, chan) (": 441 " + nick + " " + chan + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(nick, chan) (": 442 " + nick + " " + chan + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nick, chan) (": 443 " + nick + " " + chan + " :is already on channel\r\n")
#define ERR_NOTREGISTERED(nick) (": 451 " + nick + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(nick) (": 461 " + nick + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTERED(nick) (": 462 " + nick + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(nick) (": 464 " + nick + " :Password incorrect\r\n")
#define ERR_CHANNELISFULL(nick, chan) (": 471 " + nick + " " + chan + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(nick, mode) (": 472 " + nick + " " + mode + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(nick, chan) (": 473 " + nick + " " + chan + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(nick, chan) (": 475 " + nick + " " + chan + " :Cannot join channel (+k)\r\n")
#define ERR_CHANOPRIVSNEEDED(chan) (": 482 " + chan + " :You're not a channel operator\r\n")

#define RPL_CONNECTED(nick) (": 001 " + nick + " : Welcome to the IRC server\r\n")
#define RPL_JOINCHANNEL(nick, user, ipaddress, chan) (":" + nick + "!" + user + "@" + ipaddress + " JOIN " + chan + "\r\n")
#define RPL_INVITEDCHANNEL(nick, user, ipaddress, chan) (":" + nick + "!" + user + "@" + ipaddress + " INVITE " + chan + "\r\n")
#define RPL_TOPICIS(nick, chan, topic) (": 332 " + nick + " " + chan + " :" + topic + "\r\n")
#define RPL_INVITING(client, nick, chan) (": 341 " + client + " " + nick + " " + chan + "\r\n")
#define RPL_CLIENTLIST(nick, chan, clientslist) (": 353 " + nick + " = " + chan + " :" + clientslist + "\r\n")
#define RPL_ENDOFNAMES(nick, chan) (": 366 " + nick + " " + chan + " :END of /NAMES list\r\n")

#endif
