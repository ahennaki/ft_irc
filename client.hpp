class Client {
	private:
		int fd;
		std::string ipAdd;
		std::string nickName;
		std::string userName;
		std::string realName;
	public:
		Client() {};
		int GetFd() {return fd;}

		void SetFd(int fd) {this->fd = fd;}
		void setIpAdd(std::string ipadd) {ipAdd = ipadd;}
};
