#ifndef UTILS_HPP
#define UTILS_HPP

#include "server.hpp"

std::vector<std::string> splitMessage(std::string str);
std::vector<std::string> splitCmd(std::string& cmd);

bool isNickValid(std::string nick);

int toInt(std::string& input);

#endif
