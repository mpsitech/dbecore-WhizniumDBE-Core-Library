/**
	* \file Crc.h
	* CRC-16 functionality (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 5 Dec 2016
	*/

#ifndef DBECORE_CRC_H
#define DBECORE_CRC_H

#include <stdlib.h>

namespace Dbecore {
	/**
		* Crc
		*/
	class Crc {

	public:
		Crc(const unsigned short crcpoly = 0x8005, const bool bitinv = false);

	public:
		unsigned short crcpoly;
		bool bitinv;

		unsigned short crc;

	public:
		void reset();

		inline void includeByte(const unsigned char b);
		void includeBytes(const unsigned char* buf, const size_t buflen);

		void finalize();
	};
};
#endif
