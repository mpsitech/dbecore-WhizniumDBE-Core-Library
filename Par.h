/**
	* \file Par.h
	* parameter functionality (declarations)
	* \copyright (C) 2017-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 29 Apr 2017
	*/

#ifndef DBECORE_PAR_H
#define DBECORE_PAR_H

#include <sbecore/Xmlio.h>

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
			static const Sbecore::uint TIX = 1;
			static const Sbecore::uint _BOOL = 2;
			static const Sbecore::uint TINYINT = 3;
			static const Sbecore::uint UTINYINT = 4;
			static const Sbecore::uint SMALLINT = 5;
			static const Sbecore::uint USMALLINT = 6;
			static const Sbecore::uint INT = 7;
			static const Sbecore::uint UINT = 8;
			static const Sbecore::uint BLOB = 9;
			static const Sbecore::uint VBLOB = 10;

		public:
			static Sbecore::uint getIx(const std::string& sref);
			static std::string getSref(const Sbecore::uint ix);
		};

	public:
		Par(const std::string& sref = "", const Sbecore::uint ixVType = 0, Sbecore::utinyint (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const Sbecore::utinyint tix) = NULL, void (*fillFeed)(Sbecore::Xmlio::Feed& feed) = NULL, size_t len = 0);
		Par(const Par& src);
		~Par();

		Par& operator=(const Par& src);

	public:
		std::string sref;
		Sbecore::uint ixVType;

		Sbecore::utinyint (*getTixBySref)(const std::string& sref); // populated in derived classes
		std::string (*getSrefByTix)(const Sbecore::utinyint tix);
		void (*fillFeed)(Sbecore::Xmlio::Feed& feed);

		unsigned char* buf;
		size_t buflen;

	public:
		void reset();

		void setTix(const Sbecore::utinyint tix);
		Sbecore::utinyint getTix();
		void setBool(const bool b);
		bool getBool();
		void setTinyint(const Sbecore::tinyint i);
		Sbecore::tinyint getTinyint();
		void setUtinyint(const Sbecore::utinyint i);
		Sbecore::utinyint getUtinyint();
		void setSmallint(Sbecore::smallint i);
		Sbecore::smallint getSmallint();
		void setUsmallint(Sbecore::usmallint i);
		Sbecore::usmallint getUsmallint();
		void setInt(int i);
		int getInt();
		void setUint(Sbecore::uint i);
		Sbecore::uint getUint();
		void setBlob(const unsigned char* x, const size_t xlen);
		unsigned char* getBlob();
		void setVblob(const unsigned char* x, const size_t xlen);
		unsigned char* getVblob();
		size_t getLen();

		static void parsToBuf(std::map<std::string,Par>& pars, std::vector<std::string>& seqPars, unsigned char** buf, size_t& buflen);
		static std::string parsToText(std::map<std::string,Par>& pars, std::vector<std::string>& seqPars, const bool truncate = false, bool* truncated = NULL);
	};
};
#endif
