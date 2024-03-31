#ifndef TOPIC_HPP
#define TOPIC_HPP

#include <iostream>

class Topic {
	private:
		std::string topic;
		std::string nick;
		std::string time;
		//time
	
	public:
		Topic() {}
		
		std::string getName() {return topic;}
		std::string getNick() {return nick;}
		std::string getTime() {return time;}

		void setName(std::string topic) {this->topic = topic;}
		void setNick(std::string nick) {this->nick = nick;}
		void setTime(std::string time) {this->time = time;}

};

#endif
