/**
	* \file Dbe.h
	* WhizniumDBE globals (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 20 Nov 2016
	*/

#ifndef DBECORE_DBE_H
#define DBECORE_DBE_H

#include <cstdint>

#include <sbecore/Mttypes.h>
#include <sbecore/Xmlio.h>

namespace Dbecore {
	/**
		* VecDbeVAction
		*/
	namespace VecDbeVAction {
		constexpr uint8_t INV = 0x00;
		constexpr uint8_t REV = 0x01;
		constexpr uint8_t RET = 0x80;
		constexpr uint8_t NEWRET = 0x81;
		constexpr uint8_t ERR = 0xF0;
		constexpr uint8_t RTEERR = 0xF1; // should not happen if library and device source are in sync
		constexpr uint8_t CREFERR = 0xF2;
		constexpr uint8_t FWDERR = 0xF3;
		constexpr uint8_t CMDERR = 0xF4; // should not happen if library and device source are in sync
		constexpr uint8_t TOERR = 0xF5;
		constexpr uint8_t RSTERR = 0xF6;

		uint8_t getTix(const std::string& sref);
		std::string getSref(const uint8_t tix);
		std::string getTitle(const uint8_t tix);
	};

	/**
		* VecDbeVXfer
		*/
	namespace VecDbeVXfer {
		constexpr uint8_t VOID = 0x00;
		constexpr uint8_t TKN = 0x01;
		constexpr uint8_t TKNSTE = 0x02;
		constexpr uint8_t AVLBX = 0x03;
		constexpr uint8_t REQBX = 0x04;
		constexpr uint8_t ARBBX = 0x05;
		constexpr uint8_t AVLLEN = 0x06;
		constexpr uint8_t REQLEN = 0x07;
		constexpr uint8_t ARBLEN = 0x08;
		constexpr uint8_t RD = 0x09;
		constexpr uint8_t RDACK = 0x0A;
		constexpr uint8_t WR = 0x0B;
		constexpr uint8_t WRACK = 0x0C;

		uint8_t getTix(const std::string& sref);
		std::string getSref(const uint8_t tix);
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
