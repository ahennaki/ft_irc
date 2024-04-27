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
        std::string str;

        int bytes = recv(Bot.servSock(), data, 1024, 0);
        std::cout << "bytes " << bytes << std::endl;
        if (bytes > 0) {
          str = data;
          if (str.substr(0, 38) == ":irc.42.chat 464 * :Password incorrect"
          || str.substr(0, 48) == ":irc.42.chat 433 bot :Nickname is already in use") {
            std::cout << data;
            break;
          }
          std::cout << data;
        }
        else {
          std::cout << "exit";
          break;
        }
      }
    }
    close(Bot.servSock());
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
