# file Makefile
# make file for Whiznium DBE core library, release dbecore_genio
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 12 Jun 2016

SRCROOT = /home/epsi/src
LIBROOT = /home/epsi/lib

CC = g++
CPPFLAGS = -std=c++11 -g -Wall -pthread

INCS = -I$(SRCROOT)
INCS += -I/usr/include/libxml2

OBJECTS = Bufxf.o Cmd.o Crc.o Dbe.o Err.o Par.o Rst.o

LIBRARY = libdbecore

.SUFFIXES: .cpp

.cpp.o:
	$(CC) $(CPPFLAGS) $(INCS) -o $@ -c $<

all:	$(LIBRARY)

$(LIBRARY):	$(OBJECTS)
	ar rcs $(LIBRARY).a $(OBJECTS)

install:
	cp $(LIBRARY).a $(LIBROOT)/

clean: 
	rm -f *.o
