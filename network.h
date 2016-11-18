#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

class TCPServer {
public:
	TCPServer() {}
	TCPServer(const string &addr, const string &port) { init(addr, port); }
	void init(const string &addr, const string &port);
	void send(const string &msg);
	int accept();
	string recv();
private:
	int initserver(int type, const struct sockaddr *addr,
			socklen_t alen, int qlen);
	int sockfd, clfd;
};

class TCPClient {
public:
	TCPClient() {}
	TCPClient(const string &addr, const string &port) { connect(addr, port); }
	void connect(const string &addr, const string &port);
	void send(const string &msg);
	string recv();
private:
	int sockfd;
	int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen);
};


