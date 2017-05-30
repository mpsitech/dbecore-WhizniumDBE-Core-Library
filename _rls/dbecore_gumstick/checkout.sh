#!/bin/bash
# file checkout.sh
# checkout script for Whiznium DBE core library, release dbecore_gumstick
# author Alexander Wirthmueller
# date created: 9 Dec 2016
# modified: 9 Dec 2016

export set SRCROOT=/home/mpsi/src

mkdir $SRCROOT/dbecore

cp Makefile $SRCROOT/dbecore/

cp ../../*.h $SRCROOT/dbecore/
cp ../../*.cpp $SRCROOT/dbecore/
