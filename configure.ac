#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sdes-rsa], [0.1], [crazyboycjr@gmail.com])
AC_CONFIG_SRCDIR([cbc.cpp])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([-Wall foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR

# Checks for libraries.
AC_PROG_RANLIB
AC_CHECK_LIB(gmp, __gmpz_init, ,
		[AC_MSG_ERROR([GNU MP not found, see https://gmplib.org/])])
AC_CHECK_LIB(gmpxx, __gmpz_init, ,
		[AC_MSG_ERROR([GNU MP not found, see https://gmplib.org/])])

# Checks for header files.
AC_CHECK_HEADERS([gmpxx.h], AC_MSG_ERROR([GNU MP not found, see https://gmplib.org/]))

# Checks for compile flags.
AX_CHECK_COMPILE_FLAG([-std=c++14], [CXXFLAGS="$CXXFLAGS -std=c++14"])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.

# Builtin functions - required since I am lazy :)
AX_GCC_BUILTIN(__builtin_bswap32)
AX_GCC_BUILTIN(__builtin_bswap64)

# Extra Functionalities
AC_ARG_ENABLE([debug], AS_HELP_STRING([--enable-debug],
			  [build with additional debugging code]),
			  [CPPFLAGS='-g'; AC_DEFINE([DEBUG], [], [Debugging])])

# DES Number of Rounds
AC_DEFINE([NR], [16], [DES Number of Rounds])

# RSA
AC_DEFINE([NBITS], [128], [Bits of RSA n])
AC_DEFINE([PUBFILE], ["id_rsa.pub"], [Default RSA pub-key file])
AC_DEFINE([PRIFILE], ["id_rsa"], [Default RSA pri-key file])
AC_DEFINE([PEERPUB], ["peer.pub"], [Default RSA pub-key file of peer])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
