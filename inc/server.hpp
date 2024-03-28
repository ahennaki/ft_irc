#ifndef SERVER_HPP
#define SERVER_HPP

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
#include "replies.hpp"
#include "client.hpp"
#include "channel.hpp"

typedef std::vector<struct pollfd>::iterator pfd_it;
typedef std::vector<Client>::iterator client_it;
typedef std::vector<Channel>::iterator channel_it;
typedef std::vector<std::string>::iterator str_it;

class Server {
	private:
		std::string port;
		std::string password;
		int serverSocket;
		std::vector<struct pollfd> pfd;
		std::vector<Client> clients;
		std::vector<Channel> channels;
		static bool signal;
		
	public:
		Server(std::string port, std::string password);
		~Server();

		static void signalHandler(int sig) {(void)sig; signal = true;}

		void validPort();
		void validPassword();
		void startServer();
		void acceptClient();
		void getMessage(int fd);

		void addClient(int fd, std::string nickName, std::string userName, std::string realName);
		Client* getClient(int fd);
		Client* getClient(std::string nick);
		void rmClient(int fd);
		void closeFds();

		void addChannel(int fd, std::string name, std::string key);
		void addClientToChan(int fd, std::string name);
		Channel createChannel(int fd, std::string name, std::string key);
		Channel* getChannel(std::string name);
		bool	channelExist(std::string name);
		bool	clientExist(int fd, std::string name);

		void rmClientFromChans(int fd);

		void passCmd(int fd, std::vector<std::string> cmd);
		void userCmd(int fd, std::vector<std::string> cmd);
		void nickCmd(int fd, std::vector<std::string> cmd);
		bool isNickUsed(std::string nick);

		void joinCmd(int fd, std::vector<std::string> cmd);
		void partCmd(int fd, std::vector<std::string> cmd);
		void quitCmd(int fd);

		void modeCmd(int fd, std::vector<std::string> cmd);
		void modeExec(int fd, std::vector<std::string> cmd);
		void inviteOnly(char opr, std::string chan);
		void topicMode(char opr, std::string chan);
		void operatorMode(int fd, char opr, std::string chan, std::string nick);

		void execute(std::string cmd, int fd);
		void registerClient(int fd);
		
		void replies(int fd, const std::string reply);
};

#endif
