#include <iostream>
#include <string>
#include <sstream>

class Server {
	private:
		std::string port;
		std::string password;
	public:
		Server(std::string port, std::string password);
		~Server();
		void startServer();
};