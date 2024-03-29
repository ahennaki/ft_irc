#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel {
	private:
		std::string name;
		std::string key;
		std::string topic;
		std::vector<Client> admins;
		std::vector<Client> users;
		std::vector<Client> invited;

	public:
		size_t limit;
		bool i;
		bool t;
		bool k;
		bool l;

		Channel() {};
		// ~Channel() {};

		std::string getName() {return name;}
		std::string getKey() {return key;}
		std::string getTopic() {return topic;}

		void setName(std::string name) {this->name = name;}
		void setKey(std::string key) {this->key = key;}
		void setTopic(std::string topic) {this->topic = topic;}

		bool isUser(Client client);
		bool isAdmin(Client client);
		bool isInvited(Client client);
		void addUser(Client user) {users.push_back(user);}
		void addInvited(Client cli) {invited.push_back(cli);}
		void addAdmin(Client admin) {admins.push_back(admin);}
		void rmUser(Client client);
		std::string getClientList();
		size_t userNbr();
};

#endif
