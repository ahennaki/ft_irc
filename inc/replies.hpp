#ifndef REPLIES_HPP
#define REPLIES_HPP

#define ERR_NEEDMOREPARAMS(nick) (": 461 " + nick + " :Not enough parameters\r\n")
#define ERR_PASSWDMISMATCH(nick) (": 464 " + nick + " :Password incorrect\r\n")
#define ERR_ERRONEUSNICK(nick) (": 432 " + nick + " :Erroneus nickname\r\n")
#define ERR_NICKINUSE(nick) (": 433 " + nick + " :Nickname is already in use\r\n")
#define ERR_NOTREGISTERED(nick) (": 451 " + nick + " :You have not registered\r\n")
#define ERR_ALREADYREGISTERED(nick) (": 462 " + nick + " :You may not reregister\r\n")
#define ERR_CMDNOTFOUND(nick, command) (": 421 " + nick + " " + command + " :Unknown command\r\n")
#define ERR_NONICKNAME(nick) (": 431 " + nick + " :No nickname given\r\n")
#define ERR_NOSUCHCHANNEL(nick, chan) (": 403 " + nick + " " + chan + " :No such channel\r\n")
#define ERR_USERONCHANNEL(nick, chan) (": 443 " + nick + " " + chan + " :is already on channel\r\n")

#define RPL_CONNECTED(nick) (": 001 " + nick + " : Welcome to the IRC server\r\n")
#define RPL_JOINCHANNEL(nick, user, ipaddress, chan) (":" + nick + "!" + user + "@" + ipaddress + " JOIN " + chan + "\r\n")
#define RPL_CLIENTLIST(nick, chan, clientslist) (": 353 " + nick + " = " + chan + " :" + clientslist + "\r\n")
#define RPL_ENDOFNAMES(nick, chan) (": 366 " + nick + " " + chan + " :END of /NAMES list\r\n")
#define RPL_TOPICIS(nick, chan, topic) (": 332 " + nick + " " +chan + " :" + topic + "\r\n")

#endif
