# file Makefile
# make file for Whiznium DBE core library, release dbecore_mac
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 24 Jun 2019

SRCROOT = ${WHIZROOT}/src

CPP = g++
CPPFLAGS = -std=c++11 -g -Wall -pthread -fPIC

STATLIB = ar
STATLIBFLAGS = rcs
STATLIBEXT = a

DYNLIB = g++
DYNLIBFLAGS = -dynamiclib -undefined dynamic_lookup
DYNLIBEXT = dylib

INCINSTPATH = ${WHIZROOT}/src
LIBINSTPATH = ${WHIZROOT}/lib

INCPATH = -I${SRCROOT}
INCPATH += -I/usr/local/include/libxml2

OBJS = Bufxf.o Cmd.o Crc.o Dbe.o Err.o Par.o Rst.o

LIB = libdbecore

.SUFFIXES: .cpp

.cpp.o:
	${CPP} ${CPPFLAGS} ${INCPATH} -o $@ -c $<

all:	${LIB}

${LIB}:	${OBJS}
	${STATLIB} ${STATLIBFLAGS} ${LIB}.${STATLIBEXT} ${OBJS}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS} -o ${LIB}.${DYNLIBEXT}

install:
	#-mkdir $(INCINSTPATH)/dbecore
	#chmod 755 ${INCINSTPATH}/dbecore
	#cp ${SRCROOT}/dbecore/*.h ${INCINSTPATH}/dbecore/
	cp $(LIB).a $(LIBINSTPATH)/
	cp $(LIB).$(DYNLIBEXT) $(LIBINSTPATH)/
	chmod 755 ${LIBINSTPATH}/libdbecore*

clean: 
	rm -f *.o
