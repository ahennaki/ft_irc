#pragma once

#include "../inc/server.hpp"
#include "CBot.hpp"
#include <arpa/inet.h> // Include for inet_addr
#include <curl/curl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <strstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>

class Bot {
private:
  std::string pass;
  int port;
  std::string host;
  pollfd _fds;
  sockaddr_in serverAddress;
  sockaddr_in HttpAddress;
  std::string ApiBuffer;

public:
  int _servSock;
  Bot(int servSock, int ac, char **av);

  void ConnectIRC();

  std::string getResponse(std::string name);
  void error(std::string msg);
  bool Send(int socket, std::string msg);
  const int &getIrcServer();
  pollfd CreatFd(int fd);
  struct pollfd *FdData();
};

typedef struct s_usergit {
  bool existe;
  std::string name;
  std::string bio;
  std::string followers;
  std::string following;
  std::string public_repo;
  std::string location;
  std::string dateCreation;
} UserGit;

UserGit parccing(std::string Buffer);