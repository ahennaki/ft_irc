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
// #include "channel.hpp"

typedef std::vector<Client>::iterator client_it;
typedef std::vector<struct pollfd>::iterator pfd_it;


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
		bool validPort();
		void startServer();
		void acceptClient();
		void getMessage(int fd);
		void ClearClients(int fd);
		void CloseFds();
};