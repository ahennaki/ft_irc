#include "../inc/utils.hpp"

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

void Server::validPort() {
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw (std::runtime_error("Invalide port."));

	int prt = toInt(port);
	if (prt < 0 || prt > 65535)
		throw (std::runtime_error("Invalide port."));
}

void Server::validPassword() {
	if (password.empty() || password.find_first_of(" \r\t\v\n") != std::string::npos)
		throw (std::runtime_error("Invalide password."));
} 

std::vector<std::string> splitCmd(std::string& cmd)
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
