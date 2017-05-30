#!/bin/bash
# file checkout.sh
# checkout script for Whiznium DBE core library, release dbecore_che
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 23 Feb 2017

export set SRCROOT=/home/mpsitech/src

mkdir $SRCROOT/dbecore

cp Makefile $SRCROOT/dbecore/

cp ../../*.h $SRCROOT/dbecore/
cp ../../*.cpp $SRCROOT/dbecore/
