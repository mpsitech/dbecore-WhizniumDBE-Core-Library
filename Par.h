/**
	* \file Par.h
	* parameter functionality (declarations)
	* \copyright (C) 2017-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 29 Apr 2017
	*/

#ifndef DBECORE_PAR_H
#define DBECORE_PAR_H

#include <dbecore/Dbe.h>

namespace Dbecore {
	/**
		* Par
		*/
	class Par {

	public:
		/**
			* VecVType
			*/
		class VecVType {

		public:
			static constexpr uint32_t TIX = 1;
			static constexpr uint32_t _BOOL = 2;
			static constexpr uint32_t INT8 = 3;
			static constexpr uint32_t UINT8 = 4;
			static constexpr uint32_t INT16 = 5;
			static constexpr uint32_t UINT16 = 6;
			static constexpr uint32_t INT32 = 7;
			static constexpr uint32_t UINT32 = 8;
			static constexpr uint32_t BLOB = 9;
			static constexpr uint32_t VBLOB = 10;

		public:
			static uint32_t getIx(const std::string& sref);
			static std::string getSref(const uint32_t ix);
		};

	public:
		Par(const std::string& sref = "", const uint32_t ixVType = 0, uint8_t (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const uint8_t tix) = NULL, void (*fillFeed)(Sbecore::Feed& feed) = NULL, size_t len = 0);

	public:
		std::string sref;
		uint32_t ixVType;

		uint8_t (*getTixBySref)(const std::string& sref);
		std::string (*getSrefByTix)(const uint8_t tix);
		void (*fillFeed)(Sbecore::Feed& feed);

		size_t len;

	public:
		static void setTix(uint8_t* buf, const size_t ofs, const uint8_t tix);
		static uint8_t getTix(const uint8_t* buf, const size_t ofs);

		static void setBool(uint8_t* buf, const size_t ofs, const bool b);
		static bool getBool(const uint8_t* buf, const size_t ofs);

		static void setInt8(uint8_t* buf, const size_t ofs, const int8_t i);
		static int8_t getInt8(const uint8_t* buf, const size_t ofs);

		static void setUint8(uint8_t* buf, const size_t ofs, const uint8_t i);
		static uint8_t getUint8(const uint8_t* buf, const size_t ofs);

		static void setInt16(uint8_t* buf, const size_t ofs, const int16_t i);
		static int16_t getInt16(const uint8_t* buf, const size_t ofs);

		static void setUint16(uint8_t* buf, const size_t ofs, uint16_t i);
		static uint16_t getUint16(const uint8_t* buf, const size_t ofs);

		static void setInt32(uint8_t* buf, const size_t ofs, const int32_t i);
		static int32_t getInt32(const uint8_t* buf, const size_t ofs);

		static void setUint32(uint8_t* buf, const size_t ofs, const uint32_t i);
		static uint32_t getUint32(const uint8_t* buf, const size_t ofs);

		static void setBlob(uint8_t* buf, const size_t ofs, const size_t len, const unsigned char* x, const size_t xlen);
		static unsigned char* getBlob(const uint8_t* buf, const size_t ofs, const size_t len);

		static void setVblob(uint8_t* buf, const size_t ofs, const size_t len, const unsigned char* x, const size_t xlen);
		static unsigned char* getVblob(const uint8_t* buf, const size_t ofs, const size_t len);
		static size_t getVblobLen(const uint8_t* buf, const size_t ofs);

		static std::string parsToText(std::map<uint32_t, Par>& pars, uint8_t* buf, const bool truncate, bool* truncated);
	};
};
#endif
