#include "server.hpp"

int toInt(std::string& input)
{
    try
    {
        /* code */
        int i;
        std::stringstream str(input);

        str >> i;
        return i;
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

Server::Server(std::string port, std::string password) : port(port), password(password) {
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw (std::runtime_error("check port value!"));
	int prt = toInt(port);
	if (prt < 0 || prt > 65535)
		throw (std::runtime_error("check port value!"));
}