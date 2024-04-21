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
	try{
		signal(SIGINT, server.signalHandler);
		signal(SIGQUIT, server.signalHandler);
		signal(SIGTSTP, server.signalHandler);
		server.validPort();
		server.validPassword();
		server.startServer();
	}
	catch(const std::exception& e){
		server.closeFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Server closed." << std::endl;
	return 0;
}
