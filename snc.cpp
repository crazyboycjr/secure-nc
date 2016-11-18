#include "common.h"
#include "network.h"
#include "rsa.h"

#include <iostream>

extern class RSA RSA;

void serverMainloop(TCPServer &server)
{
	server.accept();
	for (;;) {
		string msg = server.recv();
		cout << msg;
		cout.flush();
		getline(cin ,msg);
		server.send(msg);
	}
}

void clientMainloop(TCPClient &client)
{
	string msg;
	msg = RSA.readFile2Str("rsa.pub");
	client.send(msg);
	msg = client.recv();
	RSA.saveFile("peer.pub", msg);

	for (;;) {
		getline(cin, msg);
		client.send(msg);
		cout << client.recv();
		cout.flush();
	}
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Usage: %s server|client <addr> <port>\n", argv[0]);
		return -1;
	}

	if (string(argv[1]) == "server") {
		TCPServer server(argv[2], argv[3]);
		serverMainloop(server);
	} else if (string(argv[1]) == "client") {
		TCPClient client(argv[2], argv[3]);
		clientMainloop(client);
	} else {
		fprintf(stderr, "param error.\n");
		return -3;
	}
	return 0;
}
