#include "../inc/utils.hpp"

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
