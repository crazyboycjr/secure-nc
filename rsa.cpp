#include "common.h"
#include "rsa.h"

#include <string>
#include <gmpxx.h>
#include <cstdlib>
#include <vector>

using namespace std;

#define BLKSZ (128 / 8)

template<typename T>
T power(T a, T n, const T &p) {
	T t = 1;
	/* n >>= 1 may cause problems */
	for (; n; n >>= 1) {
		if ((n & 1) == 1) t = t * a % p;
		a = a * a % p;
	}
	return t;
}

string RSA::readFile2Str(const string &pathname)
{
#define BUFLEN 1024
	int n;
	char buf[BUFLEN];

	FILE *fp = fopen(pathname.c_str(), "rb");
	string ret;

	while ((n = fread(buf, 1, BUFLEN, fp)) > 0) {
		ret += string(buf, n);
		if (n < BUFLEN || (n == BUFLEN && buf[BUFLEN - 1] == '\0'))
			break;
	}

	if (n < 0) {
		perror("");
		exit(-1);
	}

	fclose(fp);
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

string RSA::bigInt2Str(mpz_class a, int len)
{
	string ret;
	vector<u8> v;
	while (a != 0) {
		v.push_back((u8)((mpz_class)(a % 256)).get_si());
		a >>= 8;
	}
	for (ssize_t i = v.size() - 1; i >= 0; i--) {
		ret.push_back(v[i]);
	}
	assert(v.size() <= (size_t)len);
	for (ssize_t i = v.size(); i < len; i++)
		ret.push_back((u8)0x00);
	return ret;
}

mpz_class RSA::str2BigInt(const string &str)
{
	size_t len = str.length();
	mpz_class ret = 0;
	for (const auto &c : str) {
		ret = (ret << 8) + (u8)c;
	}
	return ret;
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
	for (auto c : str) {
		writeU8(fp, (u8)c);
	}
	fclose(fp);
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
	FILE *fp = fopen(pathname.c_str(), "rb");
	string _str;
	readString(fp, _str);
	readBigInt(fp, a);
	readBigInt(fp, n);
	fclose(fp);
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

string RSA::rsaTransform(const string &text, const mpz_class &k)
{
	string ret;
	trace(text.length());
	assert(text.length() % BLKSZ == 0);
	for (size_t i = 0; i < text.length(); i += BLKSZ) {
		mpz_class m = str2BigInt(string(text.c_str() + i, BLKSZ));
		mpz_class c = power(m, k, n);
		ret += bigInt2Str(c, BLKSZ);
	}
	return ret;
}

string RSA::format(const string &text)
{
	string ret;
	u64 len = text.length();
	ret += string((char *)&len, 8);
	len += 8;
	ret += text;
	len = BLKSZ - len % BLKSZ;
	if (len == BLKSZ)
		len = 0;
	while (len--)
		ret.push_back((u8)0x00);
	return ret;
}

string RSA::deformat(const string &text)
{
	u64 len = *(u64 *)text.c_str();
	return string(text.c_str() + 8, len);
}

string RSA::rsaEncrypt(const string &plain)
{
	return rsaTransform(format(plain), e);
}

string RSA::rsaDecrypt(const string &ciper)
{
	trace(ciper.length());
	return deformat(rsaTransform(ciper, d));
}


class RSA RSA;
