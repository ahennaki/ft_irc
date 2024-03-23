#ifndef REPLIES_HPP
#define REPLIES_HPP

#define ERR_NEEDMOREPARAMS(nickname) (": 461 " + nickname + " :Not enough parameters\r\n")
#define ERR_PASSWDMISMATCH(nickname) (": 464 " + nickname + " :Password incorrect\r\n" )
#define ERR_ERRONEUSNICK(nickname) (": 432 " + nickname + " :Erroneus nickname\r\n")
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use\r\n")
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered\r\n")

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!\r\n")

#endif
