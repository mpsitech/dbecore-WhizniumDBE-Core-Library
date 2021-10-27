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
		Par(const Par& src);
		~Par();

		Par& operator=(const Par& src);

	public:
		std::string sref;
		uint32_t ixVType;

		uint8_t (*getTixBySref)(const std::string& sref); // populated in derived classes
		std::string (*getSrefByTix)(const uint8_t tix);
		void (*fillFeed)(Sbecore::Feed& feed);

		unsigned char* buf;
		size_t buflen;

	public:
		void reset();

		void setTix(const uint8_t tix);
		uint8_t getTix();
		void setBool(const bool b);
		bool getBool();
		void setInt8(const int8_t i);
		int8_t getInt8();
		void setUint8(const uint8_t i);
		uint8_t getUint8();
		void setInt16(int16_t i);
		int16_t getInt16();
		void setUint16(uint16_t i);
		uint16_t getUint16();
		void setInt32(int32_t i);
		int32_t getInt32();
		void setUint32(uint32_t i);
		uint32_t getUint32();
		void setBlob(const unsigned char* x, const size_t xlen);
		unsigned char* getBlob();
		void setVblob(const unsigned char* x, const size_t xlen);
		unsigned char* getVblob();
		size_t getLen();

		static void parsToBuf(std::map<std::string, Par>& pars, std::vector<std::string>& seqPars, unsigned char** buf, size_t& buflen);
		static std::string parsToText(std::map<std::string, Par>& pars, std::vector<std::string>& seqPars, const bool truncate = false, bool* truncated = NULL);
	};
};
#endif
