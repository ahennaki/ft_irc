#include <iostream>
#include "server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	Server server(av[1], av[2]);
	try{
		server.startServer();
	}
	catch(const std::exception& e){
		server.CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	return 0;
}