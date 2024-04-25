#include "CBot.hpp"
#include <cstddef>
#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int ac, char **av) {

  try {
    Bot Bot(socket(AF_INET, SOCK_STREAM, 0), ac, av);
    while (1) {
      int fd = poll(Bot.FdData(), 1, 2000);
      if (fd == 0)
        continue;
      std::cout << fd << std::endl;
      if (Bot.FdData()->revents & POLLIN) {
        char data[1024] = {'\0'};

        int bytes = recv(Bot._servSock, data, 1024, 0);
        std::cout << "bytes " << bytes << std::endl;
        if (bytes > 0)
          std::cout << data;
        else {
          std::cout << "exit";
          break;
        }
      }
    }
    close(Bot._servSock);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
