noinst_LIBRARIES = libdes-cbc.a
libdes_cbc_a_SOURCES = des.cpp cbc.cpp common.h des.h

noinst_PROGRAMS = gen-key \
				  snc
gen_key_SOURCES = gen-key.cpp common.h

snc_SOURCES = snc.cpp \
			  network.h \
			  network.cpp

#noinst_PROGRAMS=cbc
#cbc_SOURCES=cbc.cpp des.cpp des.h
#cbc_LDADD=$(top_srcdir)/libdes.a

#BUILT_SOURCES=

#noinst_PROGRAMS=main
#main_SOURCES=main.cpp
#EXTRA_main_DEPENDENCIES=
#main_LDADD=$(top_srcdir)/libdes-cbc.a
