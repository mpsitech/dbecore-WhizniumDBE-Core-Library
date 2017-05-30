/**
  * \file Par.h
  * parameter functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 29 Apr 2017
  * \date modified: 29 Apr 2017
  */

#ifndef DBECORE_PAR_H
#define DBECORE_PAR_H

#include <sbecore/Xmlio.h>

using namespace Xmlio;

#include <dbecore/Dbe.h>

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
		static const uint TIX = 1;
		static const uint _BOOL = 2;
		static const uint TINYINT = 3;
		static const uint UTINYINT = 4;
		static const uint SMALLINT = 5;
		static const uint USMALLINT = 6;
		static const uint INT = 7;
		static const uint UINT = 8;
		static const uint BLOB = 9;
		static const uint VBLOB = 10;

	public:
		static uint getIx(const string& sref);
		static string getSref(const uint ix);
	};

public:
	Par(const string& sref = "", const uint ixVType = 0, utinyint (*getTixBySref)(const string& sref) = NULL, string (*getSrefByTix)(const utinyint tix) = NULL, void (*fillFeed)(Feed& feed) = NULL, size_t len = 0);
	Par(const Par& src);
	~Par();

	Par& operator=(const Par& src);

public:
	string sref;
	uint ixVType;

	utinyint (*getTixBySref)(const string& sref); // populated in derived classes
	string (*getSrefByTix)(const utinyint tix);
	void (*fillFeed)(Feed& feed);

	unsigned char* buf;
	size_t buflen;

public:
	void reset();

	void setTix(const utinyint tix);
	utinyint getTix();
	void setBool(const bool b);
	bool getBool();
	void setTinyint(const tinyint i);
	tinyint getTinyint();
	void setUtinyint(const utinyint i);
	utinyint getUtinyint();
	void setSmallint(smallint i);
	smallint getSmallint();
	void setUsmallint(usmallint i);
	usmallint getUsmallint();
	void setInt(int i);
	int getInt();
	void setUint(uint i);
	uint getUint();
	void setBlob(const unsigned char* x, const size_t xlen);
	unsigned char* getBlob();
	void setVblob(const unsigned char* x, const size_t xlen);
	unsigned char* getVblob();
	size_t getLen();

	static void parsToBuf(map<string,Par>& pars, vector<string>& seqPars, unsigned char** buf, size_t& buflen);
	static string parsToText(map<string,Par>& pars, vector<string>& seqPars, const bool truncate = false, bool* truncated = NULL);
};

#endif
