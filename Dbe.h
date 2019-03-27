/**
  * \file Dbe.h
  * WhizniumDBE globals (declarations)
  * \author Alexander Wirthmüller
  * \date created: 20 Nov 2016
  * \date modified: 22 Apr 2017
  */

#ifndef DBECORE_DBE_H
#define DBECORE_DBE_H

#include <sbecore/Mttypes.h>
#include <sbecore/Xmlio.h>

using namespace Xmlio;

/**
	* VecDbeVAction
	*/
namespace VecDbeVAction {
	const utinyint INV = 0x00;
	const utinyint REV = 0x01;
	const utinyint RET = 0x80;
	const utinyint NEWRET = 0x81;
	const utinyint ERR = 0xF0;
	const utinyint RTEERR = 0xF1; // should not happen if library and device source are in sync
	const utinyint CREFERR = 0xF2;
	const utinyint FWDERR = 0xF3;
	const utinyint CMDERR = 0xF4; // should not happen if library and device source are in sync
	const utinyint TOERR = 0xF5;
	const utinyint RSTERR = 0xF6;

	utinyint getTix(const string& sref);
	string getSref(const utinyint tix);
	string getTitle(const utinyint tix);
};

/**
	* VecDbeVXfer
	*/
namespace VecDbeVXfer {
	const utinyint VOID = 0x00;
	const utinyint TKN = 0x01;
	const utinyint TKNSTE = 0x02;
	const utinyint AVLBX = 0x03;
	const utinyint REQBX = 0x04;
	const utinyint ARBBX = 0x05;
	const utinyint AVLLEN = 0x06;
	const utinyint REQLEN = 0x07;
	const utinyint ARBLEN = 0x08;
	const utinyint RD = 0x09;
	const utinyint RDACK = 0x0A;
	const utinyint WR = 0x0B;
	const utinyint WRACK = 0x0C;

	utinyint getTix(const string& sref);
	string getSref(const utinyint tix);
};

/**
  * Dbe
  */
namespace Dbe {
	bool bigendian();

	string binToHex(unsigned char bin);
	unsigned char hexToBin(string hex);

	void hexToBuf(const string& s, unsigned char** buf, size_t& buflen);
	string bufToHex(unsigned char* buf, size_t buflen, const bool truncate = false, bool* truncated = NULL);
};

/**
  * DbeException
  */
struct DbeException {
	string err;

	DbeException(
				string err
			) {
		this->err = err;
	};
};

#endif
