#!/bin/bash
# file checkout.sh
# checkout script for Whiznium DBE core library, release dbecore_mac
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 24 Jun 2019

export set SRCROOT=${WHIZROOT}/src

mkdir ${WHIZROOT}/dbecore

cp Makefile ${WHIZROOT}/dbecore/

cp ../../*.h ${WHIZROOT}/dbecore/
cp ../../*.cpp ${WHIZROOT}/dbecore/
