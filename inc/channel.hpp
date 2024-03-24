#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel {
	private:
		std::string name;
		std::string topic;
		std::vector<Client> admins;
		std::vector<Client> users;

	public:
		Channel() {};

		std::string getName() {return name;}
		std::string getTopic() {return topic;}

		void setName(std::string name) {this->name = name;}
		void setTopic(std::string topic) {this->topic = topic;}

		bool isUser(Client client);
		bool isAdmin(Client client);
		void addAdmin(Client client) {admins.push_back(client);}
		void addUser(Client user) {users.push_back(user);}
};

#endif
