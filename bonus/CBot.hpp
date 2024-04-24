#pragma once

#include "../inc/server.hpp"
#include <arpa/inet.h> // Include for inet_addr
#include <curl/curl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>
#include "CBot.hpp"
#include <iostream>
#include <map>
#include <string>
#include <strstream>

class Bot {
private:
  int _servSock;
  std::string pass;
  int port;
  std::string host;
  std::vector<pollfd> _fds;
  sockaddr_in serverAddress;
  sockaddr_in HttpAddress;
  std::string ApiBuffer;

public:
  Bot(int servSock, int ac, char **av);

  void ConnectIRC();

  std::string getResponse(std::string name);
  void error(std::string msg);
  pollfd CreatFd(int fd);
  bool Send(int socket, std::string msg);
  const int &getIrcServer();
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