#include <iostream>
#include "server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	// Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		
	}
	catch(const std::exception& e){
		
	}
	std::cout << "The Server Closed!" << std::endl;
	return 0;
}