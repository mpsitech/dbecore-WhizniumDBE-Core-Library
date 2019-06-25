# file Makefile
# make file for Whiznium DBE core library, release dbecore_apalis
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 25 Jun 2019

SRCROOT = /home/root/src

CPP = ${TCHROOT}/arm-linux-gnueabihf-g++
CPPFLAGS = --sysroot=${SYSROOT} -std=c++11 -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -g -Wall -Os -pthread

STATLIB = ${TCHROOT}/arm-linux-gnueabihf-ar
STATLIBFLAGS = rcs
STATLIBEXT = a

DYNLIB = ${TCHROOT}/arm-linux-gnueabihf-g++
DYNLIBFLAGS = -shared
DYNLIBEXT = so

INCINSTPATH = ${SYSROOT}${WHIZROOT}/src
LIBINSTPATH = ${SYSROOT}${WHIZROOT}/lib

INCPATH = -I=${SRCROOT}
INCPATH += -I=/usr/include/libxml2

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
	#-mkdir ${INCINSTPATH}/dbecore
	#chmod 755 ${INCINSTPATH}/dbecore
	#cp ${SYSROOT}${SRCROOT}/dbecore/*.h ${INCINSTPATH}/dbecore/
	#chmod 644 ${INCINSTPATH}/dbecore/*.h
	cp ${LIB}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB}.${DYNLIBEXT} ${LIBINSTPATH}/
	chmod 755 ${LIBINSTPATH}/libdbecore*

clean: 
	rm -f *.o
