#!/bin/bash
# file checkout.sh
# checkout script for Whiznium DBE core library, release dbecore_arty
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 25 Jun 2019

export set SRCROOT=${WHIZROOT}/src

mkdir ${SYSROOT}${SRCROOT}/dbecore

cp Makefile ${SYSROOT}${SRCROOT}/dbecore/

cp ../../*.h ${SYSROOT}${SRCROOT}/dbecore/
cp ../../*.cpp ${SYSROOT}${SRCROOT}/dbecore/
