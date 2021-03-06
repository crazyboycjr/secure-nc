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
	int readPub(const string &pathname = PUBFILE);
	int readPri(const string &pathname = PRIFILE);
	void writeFile(const string &filename, const mpz_class &a, const mpz_class &n);
	string rsaEncrypt(const string &text);
	string rsaDecrypt(const string &text);
private:
	string rsaTransform(const string &text, const mpz_class &k);
	mpz_class str2BigInt(const string &str);
	string bigInt2Str(mpz_class a, int len);
	string format(const string &text);
	string deformat(const string &text);
	void readString(FILE *fp, string &str);
	void readBigInt(FILE *fp, mpz_class &a);
	void writeBigInt(FILE *fp, mpz_class a);
	void writeString(FILE *fp, const string &s);
	void writeU8(FILE *fp, u8 u);
	void writeU32(FILE *fp, u32 d);
	mpz_class n, e, d;
};
