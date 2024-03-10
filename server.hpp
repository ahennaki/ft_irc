#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <poll.h>
#include <csignal>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "client.hpp"

class Server {
	private:
		std::string port;
		std::string password;
		int serverSocket;
		std::vector<struct pollfd> pfd;
		std::vector<Client> clients;
	public:
		Server(std::string port, std::string password);
		~Server();
		void startServer();
		void acceptClient();
		void getData(int fd);
		void ClearClients(int fd);
		void CloseFds();
};