#!/bin/bash
# file checkout.sh
# checkout script for Whiznium DBE core library, release dbecore_zedboard
# author Alexander Wirthmueller
# date created: 8 Feb 2016
# modified: 12 Jun 2016

export set SRCROOT=/home/mpsitech/emb/zedboard/avnet-digilent-zedboard-2017.2/build/tmp/sysroots/plnx_arm/home/root/src

mkdir $SRCROOT/dbecore

cp Makefile $SRCROOT/dbecore/

cp ../../*.h $SRCROOT/dbecore/
cp ../../*.cpp $SRCROOT/dbecore/
