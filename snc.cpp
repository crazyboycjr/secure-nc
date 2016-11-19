#include "common.h"
#include "network.h"
#include "rsa.h"
#include "des.h"

#include <iostream>

extern class RSA RSA;

#define BUFLEN 1024

string encryptStr(const string &text, u64 key)
{
	/* tmpfs could get a faster speed */
	const string ifile = "/tmp/fi";
	const string ofile = "/tmp/fo";
	RSA.saveFile(ifile, text);

	u64 iv = (u64)rand() << 32 | rand();
	encrypt(ifile, ofile, key, iv);
	return RSA.readFile2Str(ofile);
}

string decryptStr(const string &text, u64 key)
{
	/* tmpfs could get a faster speed */
	const string ifile = "/tmp/fi";
	const string ofile = "/tmp/fo";
	RSA.saveFile(ifile, text);

	u64 iv;
	decrypt(ifile, ofile, key, iv);
	return RSA.readFile2Str(ofile);
}

string readMsg()
{
	string ret;
	char buf[BUFLEN];
	while (fgets(buf, BUFLEN, stdin)) {
		ret += buf;
		if (buf[strlen(buf) - 1] == '\n')
			break;
	}
	return ret;
}

void serverMainloop(TCPServer &server)
{
	server.accept();
	/* xchg public key */
	string msg = server.recv();
	RSA.saveFile(PEERPUB, msg);
	RSA.readPub(PEERPUB);
	msg = RSA.readFile2Str(PUBFILE);
	server.send(msg);

	RSA.readPri(PRIFILE);

	/* recv des key */
	u64 desKey = 0;
	msg = server.recv();
	dispstr(msg);
	desKey = *(u64 *)RSA.rsaDecrypt(msg).c_str();
	trace(desKey);

	for (;;) {
		msg = readMsg();
		//server.send(RSA.rsaEncrypt(msg));
		server.send(encryptStr(msg, desKey));
		msg = server.recv();
		dispstr(msg);
		//cout << RSA.rsaDecrypt(msg);
		cout << decryptStr(msg, desKey);
		cout.flush();
	}
}

void clientMainloop(TCPClient &client)
{
	/* xchg public key*/
	string msg;
	msg = RSA.readFile2Str(PUBFILE);
	client.send(msg);
	msg = client.recv();
	RSA.saveFile(PEERPUB, msg);
	RSA.readPub(PEERPUB);

	RSA.readPri(PRIFILE);

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
		//cout << RSA.rsaDecrypt(msg);
		cout << decryptStr(msg, desKey);
		cout.flush();
		msg = readMsg();
		//client.send(RSA.rsaEncrypt(msg));
		client.send(encryptStr(msg, desKey));
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
