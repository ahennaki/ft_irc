#include <iostream>
#include "inc/server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	Server server(av[1], av[2]);
	if (!server.validPort())
		return 1;
	try{
		server.startServer();
	}
	catch(const std::exception& e){
		server.closeFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Server closed." << std::endl;
	return 0;
}
