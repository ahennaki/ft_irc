#include "../inc/server.hpp"
#include "../inc/utils.hpp"

bool Server::signal = false;
Server::Server(std::string port, std::string password)
		: port(port), password(password) {}

Server::~Server() {}

void Server::startServer() {
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		throw(std::runtime_error("Error creating socket."));

	int optval = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval,
								 sizeof(optval)) == -1)
		throw(std::runtime_error("Error: setsockopt faild."));

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Error: fcntl faild."));

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(toInt(port));

	if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		throw(std::runtime_error("Error: binding to port."));

	if (listen(serverSocket, SOMAXCONN) == -1)
		throw(std::runtime_error("Error: listening on socket."));

	std::cout << "Server is lestning..." << std::endl;

	pollfd polFd = {serverSocket, POLLIN, 0};
	pfd.push_back(polFd);
	date = getCurrentTime();

	while (true) {
		if (poll(&pfd[0], pfd.size(), -1) == -1 && !signal)
			throw(std::runtime_error("Error: poll faild."));
		if (signal)
			break;
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

void Server::acceptClient() {
	Client cli;
	struct sockaddr_in serverAddr;
	socklen_t size = sizeof(serverAddr);

	int fd = accept(serverSocket, (sockaddr *)&(serverAddr), &size);
	if (fd == -1) {
		std::cout << "Error: accept failed." << std::endl;
		return;
	}

	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cout << "Error: fcntl failed." << std::endl;
		return;
	}

	pollfd polFd = {fd, POLLIN, 0};

	cli.setFd(fd);
	cli.setIpAdd(inet_ntoa((serverAddr.sin_addr)));
	clients.push_back(cli);
	pfd.push_back(polFd);

	std::cout << "Client \"" << fd << "\" Connected successfully." << std::endl;
}

void Server::getMessage(int fd) {
	std::vector<std::string> cmd;
	char msg[1024];
	// std::string buff;
	ssize_t bytes;
	Client* cli = getClient(fd);

	bzero(msg, sizeof(msg));
	bytes = recv(fd, msg, sizeof(msg) - 1, 0);
	cli->buff += msg;
	std::cout << "RECIVE MSG: " << cli->buff;

	if (bytes <= 0) {
		std::cout << "Client \"" << fd << "\" Disconnected" << std::endl;
		rmClientFromChans(fd);
		rmClient(fd);
		close(fd);
	} else {
		if (cli->buff.find_first_of("\r\n") != std::string::npos) {
			cmd = splitMessage(cli->buff);
			str_it it = cmd.begin();
			while (it != cmd.end()) {
				execute(*it, fd);
				it++;
			}
			cli->buff.clear();
		}
	}
}

void Server::execute(std::string cmd, int fd) {
	if (cmd.empty())
		return;

	std::vector<std::string> args = splitCmd(cmd);
	Client *cli = getClient(fd);
	if (!isCmd(args[0])) {
		replies(fd, ERR_UNKNOWNCOMMAND(cli->getNickname(), args[0]));
		return;
	}
	if (args[0] == "PONG")
		return;
	if (args[0] == "PASS" || args[0] == "pass")
		passCmd(fd, args);
	else if (args[0] == "NICK" || args[0] == "nick")
		nickCmd(fd, args);
	else if (args[0] == "USER" || args[0] == "user")
		userCmd(fd, args);
	else if (args[0] == "QUIT" || args[0] == "quit")
		quitCmd(fd);
	else if (cli->getRegistred()) {
		if (args[0] == "JOIN" || args[0] == "join")
			joinCmd(fd, args);
		else if (args[0] == "PART" || args[0] == "part")
			partCmd(fd, args);
		else if (args[0] == "MODE" || args[0] == "mode")
			modeCmd(fd, args);
		else if (args[0] == "INVITE" || args[0] == "invite")
			inviteCmd(fd, args);
		else if (args[0] == "TOPIC" || args[0] == "topic")
			topicCmd(fd, cmd);
		else if (args[0] == "KICK" || args[0] == "kick")
			kickCmd(fd, args);
		else if (args[0] == "PRIVMSG" || args[0] == "privmsg")
			privmsgCmd(fd, cmd);
	} else
		replies(fd, ERR_NOTREGISTERED(cli->getNickname()));
}
