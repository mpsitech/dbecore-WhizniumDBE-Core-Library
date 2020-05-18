/**
  * \file Par.cpp
  * parameter functionality (implementation)
  * \author Alexander Wirthmüller
  * \date created: 29 Apr 2017
  * \date modified: 22 Apr 2020
  */

#include "Par.h"

using namespace std;
using namespace Sbecore;
using namespace Xmlio;

/******************************************************************************
 class Par::VecVType
 ******************************************************************************/

uint Dbecore::Par::VecVType::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s.compare("tix") == 0) return TIX;
	else if (s.compare("bool") == 0) return _BOOL;
	else if (s.compare("tinyint") == 0) return TINYINT;
	else if (s.compare("utinyint") == 0) return UTINYINT;
	else if (s.compare("smallint") == 0) return SMALLINT;
	else if (s.compare("usmallint") == 0) return USMALLINT;
	else if (s.compare("int") == 0) return INT;
	else if (s.compare("uint") == 0) return UINT;
	else if (s.compare("blob") == 0) return BLOB;
	else if (s.compare("vblob") == 0) return VBLOB;

	return 0;
};

string Dbecore::Par::VecVType::getSref(
			const uint ix
		) {
	if (ix == TIX) return("tix");
	else if (ix == _BOOL) return("bool");
	else if (ix == TINYINT) return("tinyint");
	else if (ix == UTINYINT) return("utinyint");
	else if (ix == SMALLINT) return("smallint");
	else if (ix == USMALLINT) return("usmallint");
	else if (ix == INT) return("int");
	else if (ix == UINT) return("uint");
	else if (ix == BLOB) return("blob");
	else if (ix == VBLOB) return("vblob");

	return("");
};

/******************************************************************************
 class Par
 ******************************************************************************/

Dbecore::Par::Par(
			const string& sref
			, const uint ixVType
			, utinyint (*getTixBySref)(const string& sref)
			, string (*getSrefByTix)(const utinyint tix)
			, void (*fillFeed)(Feed& feed)
			, size_t len
		) {
	this->sref = sref;
	this->ixVType = ixVType;

	this->getTixBySref = getTixBySref;
	this->getSrefByTix = getSrefByTix;
	this->fillFeed = fillFeed;

	buf = NULL;
	buflen = 0;

	switch (ixVType) {
		case VecVType::TIX:
		case VecVType::_BOOL:
		case VecVType::TINYINT:
		case VecVType::UTINYINT:
			buf = new unsigned char[1];
			buflen = 1;
			break;
		case VecVType::SMALLINT:
		case VecVType::USMALLINT:
			buf = new unsigned char[2];
			buflen = 2;
			break;
		case VecVType::INT:
		case VecVType::UINT:
			buf = new unsigned char[4];
			buflen = 4;
			break;
		case VecVType::BLOB:
			if (len != 0) {
				buf = new unsigned char[len];
				buflen = len;
			};
			break;
		case VecVType::VBLOB:
			buf = new unsigned char[len+1];
			buf[0] = len;
			buflen = len+1;
	};
};

Dbecore::Par::Par(
			const Par& src
		) {
	*this = src;
};

Dbecore::Par::~Par() {
	if (buf) delete[] buf;
};

Dbecore::Par& Dbecore::Par::operator=(
			const Par& src
		) {
	sref = src.sref;
	ixVType = src.ixVType;

	getTixBySref = src.getTixBySref;
	getSrefByTix = src.getSrefByTix;
	fillFeed = src.fillFeed;

	buf = NULL;
	buflen = 0;

	if (src.buf && (src.buflen > 0)) {
		buf = new unsigned char[src.buflen];
		memcpy(buf, src.buf, src.buflen);

		buflen = src.buflen;
	};

	return(*this);
};

void Dbecore::Par::reset() {
	if (buf) {
		if (ixVType == VecVType::VBLOB) {
			delete[] buf;

			buf = new unsigned char[1];
			buf[0] = 0;
			buflen = 1;

		} else memset(buf, 0, buflen);
	};
};

void Dbecore::Par::setTix(
			const utinyint tix
		) {
	*buf = tix;
};

utinyint Dbecore::Par::getTix() {
	return *buf;
};

void Dbecore::Par::setBool(
			const bool b
		) {
	if (b) *buf = 0x55;
	else *buf = 0xAA;
};

bool Dbecore::Par::getBool() {
	return(*buf == 0x55);
};

void Dbecore::Par::setTinyint(
			const tinyint i
		) {
	*buf = i;
};

tinyint Dbecore::Par::getTinyint() {
	return *buf;
};

void Dbecore::Par::setUtinyint(
			const utinyint i
		) {
	*buf = i;
};

utinyint Dbecore::Par::getUtinyint() {
	return *buf;
};

void Dbecore::Par::setSmallint(
			smallint i
		) {
	unsigned char* ptr = (unsigned char*) &i;

	if (Dbe::bigendian()) {
		buf[0] = ptr[0];
		buf[1] = ptr[1];
	} else {
		buf[0] = ptr[1];
		buf[1] = ptr[0];
	};
};

smallint Dbecore::Par::getSmallint() {
	smallint retval;

	if (Dbe::bigendian()) {
		((unsigned char*) &retval)[0] = buf[0];
		((unsigned char*) &retval)[1] = buf[1];
	} else {
		((unsigned char*) &retval)[0] = buf[1];
		((unsigned char*) &retval)[1] = buf[0];
	};

	return retval;
};

void Dbecore::Par::setUsmallint(
			usmallint i
		) {
	unsigned char* ptr = (unsigned char*) &i;

	if (Dbe::bigendian()) {
		buf[0] = ptr[0];
		buf[1] = ptr[1];
	} else {
		buf[0] = ptr[1];
		buf[1] = ptr[0];
	};
};

usmallint Dbecore::Par::getUsmallint() {
	usmallint retval;

	retval = (buf[0] << 8) + buf[1];

	return retval;
};

void Dbecore::Par::setInt(
			int i
		) {
	unsigned char* ptr = (unsigned char*) &i;

	if (Dbe::bigendian()) {
		buf[0] = ptr[0];
		buf[1] = ptr[1];
		buf[2] = ptr[2];
		buf[3] = ptr[3];
	} else {
		buf[0] = ptr[3];
		buf[1] = ptr[2];
		buf[2] = ptr[1];
		buf[3] = ptr[0];
	};
};

int Dbecore::Par::getInt() {
	int retval;

	if (Dbe::bigendian()) {
		((unsigned char*) &retval)[0] = buf[0];
		((unsigned char*) &retval)[1] = buf[1];
		((unsigned char*) &retval)[2] = buf[2];
		((unsigned char*) &retval)[3] = buf[3];
	} else {
		((unsigned char*) &retval)[0] = buf[3];
		((unsigned char*) &retval)[1] = buf[2];
		((unsigned char*) &retval)[2] = buf[1];
		((unsigned char*) &retval)[3] = buf[0];
	};

	return retval;
};

void Dbecore::Par::setUint(
			uint i
		) {
	unsigned char* ptr = (unsigned char*) &i;

	if (Dbe::bigendian()) {
		buf[0] = ptr[0];
		buf[1] = ptr[1];
		buf[2] = ptr[2];
		buf[3] = ptr[3];
	} else {
		buf[0] = ptr[3];
		buf[1] = ptr[2];
		buf[2] = ptr[1];
		buf[3] = ptr[0];
	};
};

uint Dbecore::Par::getUint() {
	uint retval;

	retval = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];

	return retval;
};

// all BLOB/VBLOB functions generate copies
void Dbecore::Par::setBlob(
			const unsigned char* x
			, const size_t xlen
		) {
	if (xlen >= buflen) memcpy(buf, x, buflen);
	else {
		memcpy(buf, x, xlen);
		memset(&(buf[xlen]), 0, buflen-xlen);
	};
};

unsigned char* Dbecore::Par::getBlob() {
	unsigned char* retval = NULL;

	if (buflen > 0) {
		retval = new unsigned char[buflen];
		memcpy(retval, buf, buflen);
	};

	return retval;
};

void Dbecore::Par::setVblob(
			const unsigned char* x
			, const size_t xlen
		) {
	reset();

	if ((xlen > 0) && (xlen < 256)) {
		delete[] buf;

		buf = new unsigned char[xlen+1];
		buflen = xlen+1;

		buf[0] = xlen;

		memcpy(&(buf[1]), x, xlen);
	};
};

unsigned char* Dbecore::Par::getVblob() {
	unsigned char* retval = NULL;

	if (buflen > 1) {
		retval = new unsigned char[buflen-1];
		memcpy(retval, &(buf[1]), buflen-1);
	};

	return retval;
};

size_t Dbecore::Par::getLen() {
	size_t len = 0;

	if (buf) {
		len = buflen;
		if ((ixVType == VecVType::VBLOB) && (buflen > 0)) len = buflen-1;
	};

	return len;
};

void Dbecore::Par::parsToBuf(
			map<string,Par>& pars
			, vector<string>& seqPars
			, unsigned char** buf
			, size_t& buflen
		) {
	Par* par = NULL;

	size_t bufptr;

	buflen = 0;

	for (auto it = pars.begin(); it != pars.end(); it++) buflen += it->second.buflen;

	if (buflen > 0) {
		*buf = new unsigned char[buflen];
		memset(*buf, 0, buflen);
	} else *buf = NULL;

	bufptr = 0;

	for (unsigned int i = 0; i < seqPars.size(); i++) {
		auto it = pars.find(seqPars[i]);

		if (it != pars.end()) {
			par = &(it->second);

			if ((bufptr+par->buflen) <= buflen) {
				memcpy(&((*buf)[bufptr]), par->buf, par->buflen);
				bufptr += par->buflen;
			} else break;
		};
	};
};

string Dbecore::Par::parsToText(
			map<string,Par>& pars
			, vector<string>& seqPars
			, const bool truncate
			, bool* truncated
		) {
	string retval;

	Par* par = NULL;

	Feed feed;

	bool first;

	first = true;
	for (unsigned int i = 0; i < seqPars.size(); i++) {
		auto it = pars.find(seqPars[i]);

		if (it != pars.end()) {
			par = &(it->second);

			if (first) first = false;
			else retval += ",";

			retval += par->sref + "=";

			if (par->ixVType == Par::VecVType::TIX) retval += par->getSrefByTix(par->getTix());
			else if (par->ixVType == Par::VecVType::_BOOL) {
				bool b = par->getBool();
				if (b) retval += "true"; else retval += "false";

			} else if ((par->ixVType == Par::VecVType::TINYINT) || (par->ixVType == Par::VecVType::SMALLINT) || (par->ixVType == Par::VecVType::INT)) {
				int _i = 0;

				if (par->ixVType == Par::VecVType::TINYINT) _i = par->getTinyint();
				else if (par->ixVType == Par::VecVType::SMALLINT) _i = par->getSmallint();
				else if (par->ixVType == Par::VecVType::INT) _i = par->getInt();

				retval += to_string(_i);

			} else if ((par->ixVType == Par::VecVType::UTINYINT) || (par->ixVType == Par::VecVType::USMALLINT) || (par->ixVType == Par::VecVType::UINT)) {
				unsigned int ui = 0;

				if (par->ixVType == Par::VecVType::UTINYINT) ui = par->getUtinyint();
				else if (par->ixVType == Par::VecVType::USMALLINT) ui = par->getUsmallint();
				else if (par->ixVType == Par::VecVType::UINT) ui = par->getUint();

				retval += to_string(ui);

			} else if (par->ixVType == Par::VecVType::BLOB) {
				if ((par->buflen > 32) && truncate) {
					if (truncated) *truncated = true;
					retval += "(" + to_string(par->buflen) + " bytes)";
				} else {
					retval += "0x";
					if (par->buf) for (size_t j = 0; j < par->buflen; j++) retval += Dbe::binToHex(par->buf[j]);
				};

			} else if (par->ixVType == Par::VecVType::VBLOB) {
				if ((par->buflen > 33) && truncate) {
					if (truncated) *truncated = true;
					retval += "(" + to_string(par->buflen-1) + " bytes)";
				} else {
					retval += "0x";
					if (par->buf) for (size_t j = 1; j < par->buflen; j++) retval += Dbe::binToHex(par->buf[j]);
				};
			};
		};
	};

	return retval;
};
