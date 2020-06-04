/**
  * \file Crc.cpp
  * CRC-16 functionality (implementation)
  * \author Alexander Wirthmüller
  * \date created: 5 Dec 2016
  * \date modified: 22 Apr 2020
  */

#include "Crc.h"

/******************************************************************************
 class Crc
 ******************************************************************************/

Dbecore::Crc::Crc(
			const unsigned short crcpoly
			, const bool bitinv
		) {
	this->crcpoly = crcpoly;
	this->bitinv = bitinv;

	crc = 0;
};

void Dbecore::Crc::reset() {
	crc = 0;
};

void Dbecore::Crc::includeByte(
			const unsigned char b
		) {
	unsigned short newcrc;
	bool bit;

	unsigned char i;

	if (bitinv) i = 1;
	else i = 128;

	while (i != 0) {
		bit = b & i;

		newcrc = (crc << 1);
		if (bit) newcrc |= 0x0001;

		if (crc & 0x8000) crc = newcrc ^ crcpoly;
		else crc = newcrc;

		if (bitinv) i = (i << 1);
		else i = (i >> 1);
	};
};

void Dbecore::Crc::includeBytes(
			const unsigned char* buf
			, const size_t buflen
		) {
	for (unsigned int i = 0; i < buflen; i++) includeByte(buf[i]);
};

void Dbecore::Crc::finalize() {
	unsigned short parity;
	unsigned short newcrc;

	if (crcpoly == 0x8005) {
		// parallel method
		// derived from the Easics auto-generated VHDL file for appending 16bit data (here: 0x0000) to an existing CRC w/ poly 0x8005

		// calculate parity by "folding" four times
		parity = crc ^ (crc >> 8);
		parity ^= (parity >> 4);
		parity ^= (parity >> 2);
		parity ^= (parity >> 1);
		parity &= 0x0001;

		// two xor'ed left shifts
		newcrc = (crc << 1);
		newcrc ^= (newcrc << 1);

		// special bits
		newcrc ^= ((crc & 0xc000) >> 14); // bits 0..1 xor bits 14..15
		newcrc ^= ((crc & 0xc000) >> 12); // bits 2..3 xor bits 14..15
		newcrc ^= ((crc & 0x4000) << 1); // bit 15 xor bit 14

		// parity to be xor'ed with bits 15, 1, 0
		crc = newcrc ^ ((parity << 15) + (parity << 1) + parity);

	} else {
		includeByte(0x00);
		includeByte(0x00);
	};
};
