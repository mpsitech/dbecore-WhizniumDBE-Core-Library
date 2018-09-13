# file Makefile
# make file for Whiznium DBE core library, release dbecore_ungenio
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 12 Jun 2016

SRCROOT = /Users/mpsitech/src

CPP = g++
CPPFLAGS = -std=c++11 -g -Wall -pthread -fpic

STATLIB = ar
STATLIBFLAGS = rcs

DYNLIB = g++
DYNLIBFLAGS = -dynamiclib -undefined dynamic_lookup
DYNLIBEXT = dylib

INCINSTPATH = /usr/local/include
LIBINSTPATH = /usr/local/lib

INCPATH = -I$(SRCROOT)
INCPATH += -I/usr/local/include/libxml2

OBJS = Bufxf.o Cmd.o Crc.o Dbe.o Err.o Par.o Rst.o

LIB = libdbecore

.SUFFIXES: .cpp

.cpp.o:
	$(CPP) $(CPPFLAGS) $(INCPATH) -o $@ -c $<

all:	$(LIB)

$(LIB):	$(OBJS)
	$(STATLIB) $(STATLIBFLAGS) $(LIB).a $(OBJS)
	$(DYNLIB) $(DYNLIBFLAGS) $(OBJS) -o $(LIB).$(DYNLIBEXT)

install:
	-mkdir $(INCINSTPATH)/dbecore
	cp $(SRCROOT)/dbecore/*.h $(INCINSTPATH)/dbecore/
	cp $(LIB).a $(LIBINSTPATH)/
	cp $(LIB).$(DYNLIBEXT) $(LIBINSTPATH)/

clean: 
	rm -f *.o
