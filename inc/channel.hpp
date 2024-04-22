#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "topic.hpp"

class Channel {
	private:
		std::string name;
		std::string key;
		Topic topic;
		std::vector<Client> admins;
		std::vector<Client> users;
		std::vector<Client> invited;

	public:
		std::string chanDate;
		size_t limit;
		bool i;
		bool t;
		bool k;
		bool l;

		Channel() {};
		// ~Channel() {};

		std::string getName() {return name;}
		std::string getKey() {return key;}
		Topic* getTopic() {return &topic;}

		void setName(std::string name) {this->name = name;}
		void setKey(std::string key) {this->key = key;}

		bool isUser(Client client);
		bool isAdmin(Client client);
		bool isInvited(Client client);
		void addUser(Client user) {users.push_back(user);}
		void addInvited(Client cli) {invited.push_back(cli);}
		void addAdmin(Client admin) {admins.push_back(admin);}
		void rmUser(Client client);
		void changeAdmin();
		std::string getClientList();
		size_t userNbr();
};

#endif
