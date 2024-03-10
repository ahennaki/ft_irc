class Client {
	private:
		int Fd;
		std::string IPadd;
	public:
		Client(){};
		int GetFd(){return Fd;}

		void SetFd(int fd){Fd = fd;}
		void setIpAdd(std::string ipadd){IPadd = ipadd;}
};