/**
	* \file Par.cpp
	* parameter functionality (implementation)
	* \copyright (C) 2017-2020 MPSI Technologies GmbH
  * \author Alexander Wirthmüller
	* \date created: 29 Apr 2017
	*/

#include "Par.h"

using namespace std;
using namespace Sbecore;
using namespace Xmlio;

/******************************************************************************
 class Par::VecVType
 ******************************************************************************/

uint32_t Dbecore::Par::VecVType::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s == "tix") return TIX;
	if (s == "bool") return _BOOL;
	if (s == "int8") return INT8;
	if (s == "uint8") return UINT8;
	if (s == "int16") return INT16;
	if (s == "uint16") return UINT16;
	if (s == "int32") return INT32;
	if (s == "uint32") return UINT32;
	if (s == "blob") return BLOB;
	if (s == "vblob") return VBLOB;

	return 0;
};

string Dbecore::Par::VecVType::getSref(
			const uint32_t ix
		) {
	if (ix == TIX) return("tix");
	if (ix == _BOOL) return("bool");
	if (ix == INT8) return("int8");
	if (ix == UINT8) return("uint8");
	if (ix == INT16) return("int16");
	if (ix == UINT16) return("uint16");
	if (ix == INT32) return("int32");
	if (ix == UINT32) return("uint32");
	if (ix == BLOB) return("blob");
	if (ix == VBLOB) return("vblob");

	return("");
};

/******************************************************************************
 class Par
 ******************************************************************************/

Dbecore::Par::Par(
			const string& sref
			, const uint ixVType
			, uint8_t (*getTixBySref)(const string& sref)
			, string (*getSrefByTix)(const uint8_t tix)
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
		case VecVType::INT8:
		case VecVType::UINT8:
			buf = new unsigned char[1];
			buflen = 1;
			break;
		case VecVType::INT16:
		case VecVType::UINT16:
			buf = new unsigned char[2];
			buflen = 2;
			break;
		case VecVType::INT32:
		case VecVType::UINT32:
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
			buf = new unsigned char[len + 1];
			buf[0] = len;
			buflen = len + 1;
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
			const uint8_t tix
		) {
	*buf = tix;
};

uint8_t Dbecore::Par::getTix() {
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

void Dbecore::Par::setInt8(
			const int8_t i
		) {
	*buf = i;
};

int8_t Dbecore::Par::getInt8() {
	return *buf;
};

void Dbecore::Par::setUint8(
			const uint8_t i
		) {
	*buf = i;
};

uint8_t Dbecore::Par::getUint8() {
	return *buf;
};

void Dbecore::Par::setInt16(
			int16_t i
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

int16_t Dbecore::Par::getInt16() {
	int16_t retval;

	if (Dbe::bigendian()) {
		((unsigned char*) &retval)[0] = buf[0];
		((unsigned char*) &retval)[1] = buf[1];
	} else {
		((unsigned char*) &retval)[0] = buf[1];
		((unsigned char*) &retval)[1] = buf[0];
	};

	return retval;
};

void Dbecore::Par::setUint16(
			uint16_t i
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

uint16_t Dbecore::Par::getUint16() {
	uint16_t retval;

	retval = (buf[0] << 8) + buf[1];

	return retval;
};

void Dbecore::Par::setInt32(
			int32_t i
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

int32_t Dbecore::Par::getInt32() {
	int32_t retval;

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

void Dbecore::Par::setUint32(
			uint32_t i
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

uint32_t Dbecore::Par::getUint32() {
	uint32_t retval;

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
		retval = new unsigned char[buflen - 1];
		memcpy(retval, &(buf[1]), buflen - 1);
	};

	return retval;
};

size_t Dbecore::Par::getLen() {
	size_t len = 0;

	if (buf) {
		len = buflen;
		if ((ixVType == VecVType::VBLOB) && (buflen > 0)) len = buflen - 1;
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

			if ((bufptr + par->buflen) <= buflen) {
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

			} else if ((par->ixVType == Par::VecVType::INT8) || (par->ixVType == Par::VecVType::INT16) || (par->ixVType == Par::VecVType::INT32)) {
				int _i = 0;

				if (par->ixVType == Par::VecVType::INT8) _i = par->getInt8();
				else if (par->ixVType == Par::VecVType::INT16) _i = par->getInt16();
				else if (par->ixVType == Par::VecVType::INT32) _i = par->getInt32();

				retval += to_string(_i);

			} else if ((par->ixVType == Par::VecVType::UINT8) || (par->ixVType == Par::VecVType::UINT16) || (par->ixVType == Par::VecVType::UINT32)) {
				unsigned int ui = 0;

				if (par->ixVType == Par::VecVType::UINT8) ui = par->getUint8();
				else if (par->ixVType == Par::VecVType::UINT16) ui = par->getUint16();
				else if (par->ixVType == Par::VecVType::UINT32) ui = par->getUint32();

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
