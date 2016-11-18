#include "common.h"

#include <string>
#include <gmpxx.h>
#include <cstdlib>
#include <vector>

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
} RSA;

string RSA::readFile2Str(const string &pathname)
{
#define BUFLEN 1024
	int n;
	char buf[BUFLEN];

	FILE *fp = fopen(pathname.c_str(), "rb");
	string ret;

	while ((n = fread(buf, BUFLEN, 1, fp)) > 0) {
		ret += string(buf, n);
	}

	if (n < 0) {
		perror("");
		exit(-1);
	}

	return ret;
}

void RSA::writeU8(FILE *fp, u8 u)
{
	fwrite(&u, 1, 1, fp);
}

void RSA::writeU32(FILE *fp, u32 d)
{
	d = __builtin_bswap32(d);
	fwrite(&d, 4, 1, fp);
}

void RSA::writeBigInt(FILE *fp, mpz_class a)
{
	vector<u8> v;
	while (a != 0) {
		v.push_back((u8)((mpz_class)(a % 256)).get_si());
		a >>= 8;
	}
	writeU32(fp, (u32)v.size());
	for (ssize_t i = v.size() - 1; i >= 0; i--) {
		writeU8(fp, v[i]);
	}
}

void RSA::writeString(FILE *fp, const string &s)
{
	writeU32(fp, (u32)s.length());
	for (auto c : s) {
		writeU8(fp, (u8)c);
	}
}

void RSA::writeFile(const string &filename, const mpz_class &a, const mpz_class &n)
{
	FILE *fp = fopen(filename.c_str(), "wb");
	writeString(fp, "cjr-rsa");
	writeBigInt(fp, a);
	writeBigInt(fp, n);
	fclose(fp);
}

void RSA::saveFile(const string &pathname, const string &str)
{
	FILE *fp = fopen(pathname.c_str(), "wb");
	writeString(fp, str);
}

void RSA::readString(FILE *fp, string &str)
{
	int n;
	char buf[BUFLEN];
	fread(&n, 4, 1, fp);
	n = __builtin_bswap32(n);
	if (n < BUFLEN) {
		fread(buf, n, 1, fp);
		str = string(buf, n);
	} else {
		assert(0);
	}
}

void RSA::readBigInt(FILE *fp, mpz_class &a)
{
	string str;
	readString(fp, str);
	a = 0;
	for (const auto &c : str) {
		a = (a << 8) + (u8)c;
	}
}

int RSA::readFile(const string &pathname, mpz_class &a, mpz_class &n)
{
	FILE *fp = fopen(pathname.c_str(), "wb");
	string _str;
	readString(fp, _str);
	readBigInt(fp, a);
	readBigInt(fp, n);
	return 0;
}

int RSA::readPub(const string &pathname) {
	mpz_class _e, _n;
	int ret = readFile(pathname, _e, _n);
	e = _e;
	n = _n;
	return ret;
}

int RSA::readPri(const string &pathname) {
	mpz_class _d, _n;
	int ret = readFile(pathname, _d, _n);
	d = _d;
	n = _n;
	return ret;
}

string rsaEncrypt(const string &plain, const mpz_class &e, const mpz_class &n) {
	return "";
}
