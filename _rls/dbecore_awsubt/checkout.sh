#!/bin/bash
# file checkout.sh
# checkout script for Whiznium DBE core library, release dbecore_awsubt
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 12 Jun 2016

export set SRCROOT=/home/ubuntu/src

mkdir $SRCROOT/dbecore

cp Makefile $SRCROOT/dbecore/

cp ../../*.h $SRCROOT/dbecore/
cp ../../*.cpp $SRCROOT/dbecore/
