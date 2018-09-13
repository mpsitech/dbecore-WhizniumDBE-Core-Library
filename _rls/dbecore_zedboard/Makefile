# file Makefile
# make file for Whiznium DBE core library, release dbecore_zedboard
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 3 May 2018

SRCROOT = /home/root/src

CPP = /opt/Xilinx/petalinux-v2017.2/tools/linux-i386/gcc-arm-linux-gnueabi/bin/arm-linux-gnueabihf-g++
CPPFLAGS = --sysroot=/home/mpsitech/emb/zedboard/avnet-digilent-zedboard-2017.2/build/tmp/sysroots/plnx_arm -std=c++11 -mfloat-abi=hard -g -Wall -Os -pthread -fpic

STATLIB = /opt/Xilinx/petalinux-v2017.2/tools/linux-i386/gcc-arm-linux-gnueabi/bin/arm-linux-gnueabihf-ar
STATLIBFLAGS = rcs

DYNLIB = /opt/Xilinx/petalinux-v2017.2/tools/linux-i386/gcc-arm-linux-gnueabi/bin/arm-linux-gnueabihf-g++
DYNLIBFLAGS = -shared
DYNLIBEXT = so

INCINSTPATH = /home/mpsitech/emb/zedboard/avnet-digilent-zedboard-2017.2/build/tmp/sysroots/plnx_arm/usr/local/include
LIBINSTPATH = /home/mpsitech/emb/zedboard/avnet-digilent-zedboard-2017.2/build/tmp/sysroots/plnx_arm/usr/local/lib

INCPATH = -I=$(SRCROOT)
INCPATH += -I=/usr/include/libxml2

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
	chmod 755 $(INCINSTPATH)/dbecore
	cp /home/mpsitech/emb/zedboard/avnet-digilent-zedboard-2017.2/build/tmp/sysroots/plnx_arm/$(SRCROOT)/dbecore/*.h $(INCINSTPATH)/dbecore/
	cp $(LIB).a $(LIBINSTPATH)/
	cp $(LIB).$(DYNLIBEXT) $(LIBINSTPATH)/
	chmod 755 $(LIBINSTPATH)/libdbecore*

clean: 
	rm -f *.o
