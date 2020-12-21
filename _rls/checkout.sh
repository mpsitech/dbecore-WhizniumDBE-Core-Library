#!/bin/bash
# file checkout.sh
# checkout script for Whiznium DBE core library
# copyright: (C) 2016-2020 MPSI Technologies GmbH
# author: Alexander Wirthmueller
# date created: 8 Feb 2016

export set BUILDROOT=${WHIZSDKROOT}/build

mkdir ${SYSROOT}${BUILDROOT}/dbecore

cp ../*.h ${SYSROOT}${BUILDROOT}/dbecore/
cp ../*.cpp ${SYSROOT}${BUILDROOT}/dbecore/
