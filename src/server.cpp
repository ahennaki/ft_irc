#include "../inc/server.hpp"
#include "../inc/utils.hpp"

static int toInt(std::string& input)
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

Server::Server(std::string port, std::string password) : port(port), password(password) {}

bool Server::validPort(){
	if (port.find_first_not_of("0123456789") != std::string::npos) {
		std::cerr << "invalide port: \"should be integer\"" << std::endl;
		return false;
	}
	int prt = toInt(port);
	if (prt < 0 || prt > 65535) {
		std::cerr << "invalide port: \"should be between 0 and 65535\"" << std::endl;
		return false;
	}
	return true;
}

Server::~Server() {}

void Server::startServer() {
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		throw (std::runtime_error("Error creating socket."));

	int optval = 0;
	if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		throw(std::runtime_error("Error: setsockopt faild."));

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Error: fcntl faild."));

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(toInt(port));

	if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw (std::runtime_error("Error: binding to port."));

	if (listen(serverSocket, SOMAXCONN) == -1)
		throw (std::runtime_error("Error: listening on socket."));
	
	std::cout << "Server is lestning..." << std::endl;

	pollfd polFd = {serverSocket, POLLIN, 0};
    pfd.push_back(polFd);

	while (true) {
		if (poll(&pfd[0], pfd.size(), -1) == -1)
			throw (std::runtime_error("Error: poll faild."));
		for (size_t i = 0; i < pfd.size(); i++) {
			if (pfd[i].revents && POLLIN) {
				if (pfd[i].fd == serverSocket)
					acceptClient();
				else
					getMessage(pfd[i].fd);
			}
		}
	}
	closeFds();
}

void Server::acceptClient()
{
	Client cli;
	struct sockaddr_in serverAddr;
	socklen_t size = sizeof(serverAddr);

	int incofd = accept(serverSocket, (sockaddr *)&(serverAddr), &size);
	if (incofd == -1) {
		std::cout << "accept() failed." << std::endl;
		return;
	}

	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) {
		std::cout << "fcntl() failed." << std::endl;
		return;
	}

	pollfd polFd = {incofd, POLLIN, 0};

	cli.setFd(incofd);
	cli.setIpAdd(inet_ntoa((serverAddr.sin_addr)));
	clients.push_back(cli);
	pfd.push_back(polFd);

	std::cout << "Client \"" << incofd << "\" Connected successfully." << std::endl;
}

void Server::getMessage(int fd)
{
	std::vector<std::string> cmd;
	char msg[1024];
	bzero(msg, sizeof(msg));

	ssize_t bytes = recv(fd, msg, sizeof(msg) - 1 , 0);

	if (bytes <= 0) {
		std::cout << "Client \"" << fd << "\" Disconnected" << std::endl;
		clearClient(fd);
		close(fd);
	}
	else {
		cmd = splitMessage(msg);
		str_it it = cmd.begin();
		while (it != cmd.end()) {
			execute(*it, fd);
			it++;
		}
		// std::cout << "Client \"" << fd << "\" send message: " << buff;
	}
}

void Server::execute(std::string cmd, int fd) {
	std::vector<std::string> str = splitCmd(cmd);
	if (str[0] == "PASS")
		passCmd(fd, str);
	else if (str[0] == "NICK")
		nickCmd(fd, str);
}

void Server::replies(int fd, std::string reply)
{
	std::cout << "Response:\n" << reply;
	if(send(fd, reply.c_str(), reply.size(), 0) == -1)
		std::cerr << "Response send() faild" << std::endl;
}


void Server::registerClient(int fd) {
	if (getClient(fd)->getAuth() && !(getClient(fd)->getNickname()).empty() && !(getClient(fd)->getUsername()).empty())
		getClient(fd)->setRegistred(true);
}

void Server::clearClient(int fd) {
	pfd_it p_it = pfd.begin();
	while (p_it != pfd.end()) {
		if ((*p_it).fd == fd) {
			pfd.erase(p_it);
			break;
		}
		p_it++;
	}

	client_it it = clients.begin();
	while (it != clients.end()) {
		if ((*it).getFd() == fd) {
			clients.erase(it);
			break;
		}
		it++;
	}
}

void Server::closeFds(){
	client_it it = clients.begin();
	while (it != clients.end()) {
		std::cout << "Client \"" << (*it).getFd() << "\" Disconnected" << std::endl;
		close((*it).getFd());
	}
	if (serverSocket != -1) {
		std::cout << "Server \"" << serverSocket << "\" Disconnected" << std::endl;
		close(serverSocket);
	}
}
