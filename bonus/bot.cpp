#include "CBot.hpp"
#include <curl/curl.h>
#include <exception>
#include <iostream>


int main(int ac, char **av) {
  
  try {
    Bot Bot(socket(AF_INET, SOCK_STREAM, 0), ac, av);
    while (1) {
    }
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}