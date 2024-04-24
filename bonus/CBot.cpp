#include "CBot.hpp"
#include "../inc/server.hpp"
#include <arpa/inet.h> // Include for inet_addr
#include <curl/curl.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
//...
pollfd Bot::CreatFd(int fd) {
  pollfd _fd;
  _fd.fd = fd;
  _fd.events = POLLIN;
  _fd.revents = 0;
  return _fd;
}
const int &Bot::getIrcServer() { return (this->_servSock); }

bool Bot::Send(int socket, std::string msg) {
  send(socket, msg.c_str(), msg.length(), 0);
  return true;
}

void Bot::ConnectIRC() {
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(this->port);
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(this->getIrcServer(), (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) == -1)
    throw std::runtime_error("Error connecting to the irc server");

  Send(this->getIrcServer(), "PASS " + pass + "\nNICK bot\nUSER bot 0 * bot\n");
}

Bot::Bot(int servSock, int ac, char **av) {

  if (ac != 3)
    throw std::runtime_error("Arguments not valid !!");
  std::stringstream testpor(av[1]);
  if (!(testpor >> this->port))
    throw std::runtime_error("PORT not valid !!");
  this->pass = av[2];
  this->_servSock = servSock;
  _fds.push_back(CreatFd(servSock));
  ConnectIRC();
}
