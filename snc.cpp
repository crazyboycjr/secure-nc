#include "common.h"
#include "network.h"
#include "rsa.h"
#include "des.h"

#include <iostream>

extern class RSA RSA;

void serverMainloop(TCPServer &server)
{
	server.accept();
	/* xchg public key */
	string msg = server.recv();
	RSA.saveFile("peer.pub", msg);
	RSA.readPub("peer.pub");
	msg = RSA.readFile2Str("id_rsa.pub");
	server.send(msg);

	RSA.readPri("id_rsa");

	/* recv des key */
	u64 desKey = 0;
	msg = server.recv();
	dispstr(msg);
	desKey = *(u64 *)RSA.rsaDecrypt(msg).c_str();
	trace(desKey);

	for (;;) {
		getline(cin ,msg);
		server.send(RSA.rsaEncrypt(msg));
		msg = server.recv();
		dispstr(msg);
		cout << RSA.rsaDecrypt(msg);
		cout.flush();
	}
}

void clientMainloop(TCPClient &client)
{
	/* xchg public key*/
	string msg;
	msg = RSA.readFile2Str("id_rsa.pub");
	client.send(msg);
	msg = client.recv();
	RSA.saveFile("peer.pub", msg);
	RSA.readPub("peer.pub");

	RSA.readPri("id_rsa");

	/* send des key */
	u64 desKey = randomDESKey();
	msg = string((char *)&desKey, 8);
	dispstr(msg);
	string enMsg = RSA.rsaEncrypt(msg);
	dispstr(enMsg);
	client.send(RSA.rsaEncrypt(msg));

	for (;;) {
		msg = client.recv();
		dispstr(msg);
		cout << RSA.rsaDecrypt(msg);
		cout.flush();
		getline(cin, msg);
		client.send(RSA.rsaEncrypt(msg));
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
