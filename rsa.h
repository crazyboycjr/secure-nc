#pragma once

#include "common.h"

#include <string>
#include <gmpxx.h>

using namespace std;

class RSA {
public:
	string readFile2Str(const string &pathname);
	void saveFile(const string &pathname, const string &msg);
	int readFile(const string &pathname, mpz_class &a, mpz_class &n);
	int readPub(const string &pathname = "id_rsa.pub");
	int readPri(const string &pathname = "id_rsa");
	void writeFile(const string &filename, const mpz_class &a, const mpz_class &n);
private:
	void readString(FILE *fp, string &str);
	void readBigInt(FILE *fp, mpz_class &a);
	void writeBigInt(FILE *fp, mpz_class a);
	void writeString(FILE *fp, const string &s);
	void writeU8(FILE *fp, u8 u);
	void writeU32(FILE *fp, u32 d);
	mpz_class n, e, d;
};
