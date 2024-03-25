#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel {
	private:
		std::string name;
		std::string key;
		std::string topic;
		std::vector<Client> users;

	public:
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
		std::string getClientList();
};

#endif
