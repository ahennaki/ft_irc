#include "../inc/utils.hpp"
#include <ctime>

std::string getCurrentTime() {
  std::time_t currentTime;
  std::time(&currentTime);
  std::string time(std::ctime(&currentTime));
  return time.erase(time.size() - 1);
}

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

std::string toString(size_t& input)
{
    try
    {
        std::stringstream str;

        str << input;

        return str.str();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}


void Server::validPort() {
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw (std::runtime_error("Invalide port."));

	int prt = toInt(port);
	if (prt < 1024 || prt > 65535)
		throw (std::runtime_error("Invalide port."));
}

void Server::validPassword() {
	if (password.empty() || password.find_first_of(" \r\t\v\n") != std::string::npos)
		throw (std::runtime_error("Invalide password."));
} 

std::vector<std::string> splitCmd(std::string cmd)
{
	std::vector<std::string> vec;
	std::istringstream ss(cmd);
	std::string token;

	while(ss >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return vec;
}

std::vector<std::string> splitMessage(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream ss(str);
	std::string line;

	while(std::getline(ss, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}

bool Server::isNickUsed(std::string nick) {
	client_it it = clients.begin();
	while (it != clients.end()) {
		if (!((*it).getNickname()).compare(nick))
			return true;
		it++;
	}
	return false;
}

bool isNickValid(std::string nick) {
	if (nick[0] == '#' || nick[0] == ':' || nick[0] == '&')
		return false;
	for (size_t i = 0; i < nick.size(); i++)
		if (!std::isalnum(nick[i]))
			return false;
	return true;
}

bool isCmd(std::string cmd) {
	return (cmd == "PASS" || cmd == "NICK" || cmd == "USER" || cmd == "QUIT"
			|| cmd == "JOIN" || cmd == "PART" || cmd == "KICK" || cmd == "TOPIC"
			|| cmd == "INVITE" || cmd == "MODE" || cmd == "PRIVMSG"
			|| cmd == "pass" || cmd == "nick" || cmd == "user" || cmd == "quit"
			|| cmd == "join" || cmd == "part" || cmd == "kick" || cmd == "topic"
			|| cmd == "invite" || cmd == "mode" || cmd == "privmsg" || cmd == "PONG");
}

std::vector<std::string> ft_split(std::string cmd, char delimiter) {
	std::vector<std::string> tokens;
	std::string str;
	size_t pos = 0;

	while (pos < cmd.size()) {
		if (cmd[pos] != delimiter)
			str += cmd[pos];
		else {
			tokens.push_back(str);
			str.clear();
		}
		++pos;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}

void Server::closeFds(){
	client_it it = clients.begin();
	while (it != clients.end()) {
		std::cout << "Client \"" << (*it).getFd() << "\" Disconnected" << std::endl;
		close((*it).getFd());
		it++;
	}

	if (serverSocket != -1) {
		std::cout << "Server \"" << serverSocket << "\" Disconnected" << std::endl;
		close(serverSocket);
	}
	
}

void Server::sendToAllUser(int fd, Channel* ch, std::string reply) {
	client_it it = clients.begin();
	while (it != clients.end()) {
		if ((ch->isAdmin(*it) || ch->isUser(*it)) && it->getFd() != fd)
			sendReplieToClient((*it).getFd(), reply);
		it++;
	}
}

void Server::sendReplieToClient(int fd, std::string reply) {
	if(send(fd, reply.c_str(), reply.size(), 0) == -1)
		std::cerr << "Error: send faild" << std::endl;
}

void Server::replies(int fd, std::string reply)
{
	std::cout << reply;
	if(send(fd, reply.c_str(), reply.size(), 0) == -1)
		std::cerr << "Error: send faild" << std::endl;
}

bool isMode(char m) {return m == 'i' || m == 't' || m == 'o' || m == 'l' || m == 'k' ;}
