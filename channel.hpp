#include <iostream>

class Channel {
	private:
		std::string name;
		std::string topic;

	public:
		Channel() {};

		std::string getName() {return name;}
		std::string getTopic() {return topic;}

		void setName(std::string name) {this->name = name;}
		void setTopic(std::string topic) {this->topic = topic;}
};
