/**
	* \file Dbe.h
	* WhizniumDBE globals (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 20 Nov 2016
	*/

#ifndef DBECORE_DBE_H
#define DBECORE_DBE_H

#include <sbecore/Mttypes.h>
#include <sbecore/Xmlio.h>

namespace Dbecore {
	/**
		* VecDbeVAction
		*/
	namespace VecDbeVAction {
		const Sbecore::utinyint INV = 0x00;
		const Sbecore::utinyint REV = 0x01;
		const Sbecore::utinyint RET = 0x80;
		const Sbecore::utinyint NEWRET = 0x81;
		const Sbecore::utinyint ERR = 0xF0;
		const Sbecore::utinyint RTEERR = 0xF1; // should not happen if library and device source are in sync
		const Sbecore::utinyint CREFERR = 0xF2;
		const Sbecore::utinyint FWDERR = 0xF3;
		const Sbecore::utinyint CMDERR = 0xF4; // should not happen if library and device source are in sync
		const Sbecore::utinyint TOERR = 0xF5;
		const Sbecore::utinyint RSTERR = 0xF6;

		Sbecore::utinyint getTix(const std::string& sref);
		std::string getSref(const Sbecore::utinyint tix);
		std::string getTitle(const Sbecore::utinyint tix);
	};

	/**
		* VecDbeVXfer
		*/
	namespace VecDbeVXfer {
		const Sbecore::utinyint VOID = 0x00;
		const Sbecore::utinyint TKN = 0x01;
		const Sbecore::utinyint TKNSTE = 0x02;
		const Sbecore::utinyint AVLBX = 0x03;
		const Sbecore::utinyint REQBX = 0x04;
		const Sbecore::utinyint ARBBX = 0x05;
		const Sbecore::utinyint AVLLEN = 0x06;
		const Sbecore::utinyint REQLEN = 0x07;
		const Sbecore::utinyint ARBLEN = 0x08;
		const Sbecore::utinyint RD = 0x09;
		const Sbecore::utinyint RDACK = 0x0A;
		const Sbecore::utinyint WR = 0x0B;
		const Sbecore::utinyint WRACK = 0x0C;

		Sbecore::utinyint getTix(const std::string& sref);
		std::string getSref(const Sbecore::utinyint tix);
	};

	/**
		* Dbe
		*/
	namespace Dbe {
		bool bigendian();

		std::string binToHex(unsigned char bin);
		unsigned char hexToBin(std::string hex);

		void hexToBuf(const std::string& s, unsigned char** buf, size_t& buflen);
		std::string bufToHex(unsigned char* buf, size_t buflen, const bool truncate = false, bool* truncated = NULL);
	};

	/**
		* DbeException
		*/
	struct DbeException {
		std::string err;

		DbeException(
					std::string err
				) {
			this->err = err;
		};
	};
};
#endif
