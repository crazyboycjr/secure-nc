#include "common.h"

#include <gmpxx.h>

using namespace std;

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

template<typename T>
bool testPrime(const T &p, const T &a) {
	T m = p - 1, t;
	for (; (m & 1) == 0 && (t = power(a, m, p)) == 1; m >>= 1);
	return t == 1 || t == p - 1;
}

template<typename T>
bool millerRabin(const T &x) {
	if (x == 2) return true;
	/* x < 3,317,044,064,679,887,385,961,981 */
	const T a[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
	for (const T &i : a) {
		if (!testPrime(x, i))
			return false;
	}
	return true;
}

mpz_class genPrimeBits(int nbits) {
	mpz_class p;
	gmp_randclass r(gmp_randinit_default);
	r.seed((long)&r);

	do {
		p = r.get_z_bits(nbits);
	} while (!millerRabin(p));
	return p;
}

bool chkStrongPrime(mpz_class p) {
	/* TODO */
	return false;
}

mpz_class genStrongPrimeBits(int nbits) {
	mpz_class p;

	do {
		p = genPrimeBits(nbits);
	} while (chkStrongPrime(p));

	return p;
}

template<typename T>
T ex_gcd(const T &a, const T &b, T &x, T &y) {
	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	T c = a % b;
	T d = ex_gcd(b, (T)(a % b), x, y);
	T t = x;
	x = y;
	y = t - a / b * y;
	return d;
}

int genkeyRSA(int nbits, const string &pubname, const string &priname) {
	if (!(nbits >= 64 && nbits <= 4096 && nbits == (nbits & -nbits))) {
		printf("Not support for %d\n bits key", nbits);
		return 1;
	}

	mpz_class p, q, n, r, e, d;
	e = 65537_mpz;
	do {
		p = genStrongPrimeBits(nbits >> 1);
		q = genStrongPrimeBits(nbits >> 1);
		r = (p - 1) * (q - 1);
	} while (r % e == 0);
	n = p * q;

	mpz_class t;
	assert(1 == ex_gcd(e, r, d, t));

	d = (d % r + r) % r;
	trace(e, d, n, r);
	assert(d * e % r == 1);

	writeFile(pubname, e, n);
	writeFile(priname, d, n);

	return 0;
}

//__attribute__ ((visibility(".hidden")))
static void test() {
	assert(millerRabin(99991l));
	assert(millerRabin(99991_mpz));
	assert(millerRabin(1000000007_mpz));
	
	mpz_class a, b, c;
	a = 999999999999999999999999999123123123_mpz;
	b = 1231284093840982304829384_mpz;
	assert(a == power(a, mpz_class(3), mpz_class(a+1)));

	gmp_randclass r(gmp_randinit_default);
	r.seed((long)&r);
	mpz_class n = 0x100000000000000000000000000000000_mpz; // 128bit
	trace(r.get_z_range(n));

	// test >>
	n = 125;
	assert((n >> 1) == n / 2);

	assert(millerRabin(genPrimeBits(64)));
	assert(millerRabin(genStrongPrimeBits(64)));

	genkeyRSA(128, "id_rsa.pub", "id_rsa");
}

int main() {
	test();
	return 0;
}
