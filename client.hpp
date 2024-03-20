class Client {
	private:
		int fd;
		std::string ipAdd;
		std::string nickName;
		std::string userName;
		std::string realName;
		bool auth;
	public:
		Client() : auth(0) {};
		int getFd() {return fd;}
		std::string getIpadd() {return ipAdd;}
		std::string getNickname() {return nickName;}
		std::string getUsername() {return userName;}
		std::string getRealname() {return realName;}
		bool getAuth() {return auth;}

		void setFd(int fd) {this->fd = fd;}
		void setIpAdd(std::string ipadd) {ipAdd = ipadd;}
		void setNickname(std::string nickName) {this->nickName = nickName;}
		void setUsername(std::string userName) {this->userName = userName;}
		void setRealname(std::string realName) {this->realName = realName;}
		void setAuth(bool auth) {this->auth = auth;}
};
