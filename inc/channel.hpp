#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel {
	private:
		std::string name;
		std::string key;
		std::string topic;
		std::vector<Client> admins;
		std::vector<Client> users;

	public:
		int limit = 0;
		bool i = false;
		bool t = false;
		bool k = false;
		bool l = false;

		Channel() {};

		std::string getName() {return name;}
		std::string getKey() {return key;}
		std::string getTopic() {return topic;}

		void setName(std::string name) {this->name = name;}
		void setKey(std::string key) {this->key = key;}
		void setTopic(std::string topic) {this->topic = topic;}

		bool isUser(Client client);
		bool isAdmin(Client client);
		void addUser(Client user) {users.push_back(user);}
		void addAdmin(Client admin) {admins.push_back(admin);}
		void rmUser(Client client);
		std::string getClientList();
};

#endif
