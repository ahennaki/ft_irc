#include <iostream>
#include <vector>
#include "client.hpp"

typedef std::vector<Client>::iterator admin_it;

class Channel {
	private:
		std::string name;
		std::string topic;
		std::vector<Client> admins;

	public:
		Channel() {};

		std::string getName() {return name;}
		std::string getTopic() {return topic;}

		void setName(std::string name) {this->name = name;}
		void setTopic(std::string topic) {this->topic = topic;}

		bool isAdmin(Client client);
		void addAdmin(Client client) {admins.push_back(client);}
};
