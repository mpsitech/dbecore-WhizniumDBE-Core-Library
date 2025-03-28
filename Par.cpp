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
			, const uint32_t ixVType
			, uint8_t (*getTixBySref)(const string& sref)
			, string (*getSrefByTix)(const uint8_t tix)
			, void (*fillFeed)(Feed& feed)
			, size_t len
		) :
			sref(sref)
			, ixVType(ixVType)
			, getTixBySref(getTixBySref)
			, getSrefByTix(getSrefByTix)
			, fillFeed(fillFeed)
			, len(len)
		{
	switch (ixVType) {
		case VecVType::TIX:
		case VecVType::_BOOL:
		case VecVType::INT8:
		case VecVType::UINT8:
			this->len = 1;
			break;
		case VecVType::INT16:
		case VecVType::UINT16:
			this->len = 2;
			break;
		case VecVType::INT32:
		case VecVType::UINT32:
			this->len = 4;
			break;
		case VecVType::BLOB:
			break;
		case VecVType::VBLOB:
			this->len++;
			break;
	};
};

void Dbecore::Par::setTix(
			uint8_t* buf
			, const size_t ofs
			, const uint8_t tix
		) {
	buf[ofs] = tix;
};

uint8_t Dbecore::Par::getTix(
			const uint8_t* buf
			, const size_t ofs
		) {
	return buf[ofs];
};

void Dbecore::Par::setBool(
			uint8_t* buf
			, const size_t ofs
			, const bool b
		) {
	buf[ofs] = ((b) ? 0x55 : 0xAA);
};

bool Dbecore::Par::getBool(
			const uint8_t* buf
			, const size_t ofs
		) {
	return(buf[ofs] == 0x55);
};

void Dbecore::Par::setInt8(
			uint8_t* buf
			, const size_t ofs
			, const int8_t i
		) {
	buf[ofs] = i;
};

int8_t Dbecore::Par::getInt8(
			const uint8_t* buf
			, const size_t ofs
		) {
	return buf[ofs];
};

void Dbecore::Par::setUint8(
			uint8_t* buf
			, const size_t ofs
			, const uint8_t i
		) {
	buf[ofs] = i;
};

uint8_t Dbecore::Par::getUint8(
			const uint8_t* buf
			, const size_t ofs
		) {
	return buf[ofs];
};

void Dbecore::Par::setInt16(
			uint8_t* buf
			, const size_t ofs
			, int16_t i
		) {
	*((uint16_t*) &(buf[ofs])) = htobe16(i);
};

int16_t Dbecore::Par::getInt16(
			const uint8_t* buf
			, const size_t ofs
		) {
	return be16toh(*((uint16_t*) &(buf[ofs])));
};

void Dbecore::Par::setUint16(
			uint8_t* buf
			, const size_t ofs
			, const uint16_t i
		) {
	*((uint16_t*) &(buf[ofs])) = htobe16(i);
};

uint16_t Dbecore::Par::getUint16(
			const uint8_t* buf
			, const size_t ofs
		) {
	return be16toh(*((uint16_t*) &(buf[ofs])));
};

void Dbecore::Par::setInt32(
			uint8_t* buf
			, const size_t ofs
			, const int32_t i
		) {
	*((uint32_t*) &(buf[ofs])) = htobe32(i);
};

int32_t Dbecore::Par::getInt32(
			const uint8_t* buf
			, const size_t ofs
		) {
	return be32toh(*((uint32_t*) &(buf[ofs])));
};

void Dbecore::Par::setUint32(
			uint8_t* buf
			, const size_t ofs
			, const uint32_t i
		) {
	*((uint32_t*) &(buf[ofs])) = htobe32(i);
};

uint32_t Dbecore::Par::getUint32(
			const uint8_t* buf
			, const size_t ofs
		) {
	return be32toh(*((uint32_t*) &(buf[ofs])));
};

// getBlob() / getVblob() allocate new memory
void Dbecore::Par::setBlob(
			uint8_t* buf
			, const size_t ofs
			, const size_t len
			, const unsigned char* x
			, const size_t xlen
		) {
	memset(&(buf[ofs]), 0, len);

	if (xlen >= len) memcpy(&(buf[ofs]), x, len);
	else memcpy(&(buf[ofs]), x, xlen);
};

unsigned char* Dbecore::Par::getBlob(
			const uint8_t* buf
			, const size_t ofs
			, const size_t len
		) {
	unsigned char* retval = NULL;

	if (len > 0) {
		retval = new unsigned char[len];
		memcpy(retval, &(buf[ofs]), len);
	};

	return retval;
};

void Dbecore::Par::setVblob(
			uint8_t* buf
			, const size_t ofs
			, const size_t len
			, const unsigned char* x
			, const size_t xlen
		) {
	memset(&(buf[ofs]), 0, len);

	if ((xlen > 0) && (xlen < 256)) {
		if ((xlen+1) >= len) {
			buf[ofs] = len - 1;
			memcpy(&(buf[ofs+1]), x, len-1);

		} else {
			buf[ofs] = xlen;
			memcpy(&(buf[ofs]), x, xlen);
		};
	};
};

unsigned char* Dbecore::Par::getVblob(
			const uint8_t* buf
			, const size_t ofs
			, const size_t len
		) {
	unsigned char* retval = NULL;

	if (len > 1) {
		retval = new unsigned char[len - 1];
		memcpy(retval, &(buf[ofs+1]), len - 1);
	};

	return retval;
};

size_t Dbecore::Par::getVblobLen(
			const uint8_t* buf
			, const size_t ofs
		) {
	return buf[ofs];
};

string Dbecore::Par::parsToText(
			map<uint32_t, Par>& pars
			, uint8_t* buf
			, const bool truncate
			, bool* truncated
		) {
	string retval;

	Par* par = NULL;

	Feed feed;

	bool first;

	first = true;

	for (auto it = pars.begin(); it != pars.end(); it++) {
		par = &(it->second);

		if (first) first = false;
		else retval += ",";

		retval += par->sref + "=";

		if (par->ixVType == Par::VecVType::TIX) retval += par->getSrefByTix(getTix(buf, it->first));
		else if (par->ixVType == Par::VecVType::_BOOL) {
			bool b = getBool(buf, it->first);
			if (b) retval += "true"; else retval += "false";

		} else if ((par->ixVType == Par::VecVType::INT8) || (par->ixVType == Par::VecVType::INT16) || (par->ixVType == Par::VecVType::INT32)) {
			int _i = 0;

			if (par->ixVType == Par::VecVType::INT8) _i = getInt8(buf, it->first);
			else if (par->ixVType == Par::VecVType::INT16) _i = getInt16(buf, it->first);
			else if (par->ixVType == Par::VecVType::INT32) _i = getInt32(buf, it->first);

			retval += to_string(_i);

		} else if ((par->ixVType == Par::VecVType::UINT8) || (par->ixVType == Par::VecVType::UINT16) || (par->ixVType == Par::VecVType::UINT32)) {
			unsigned int ui = 0;

			if (par->ixVType == Par::VecVType::UINT8) ui = getUint8(buf, it->first);
			else if (par->ixVType == Par::VecVType::UINT16) ui = getUint16(buf, it->first);
			else if (par->ixVType == Par::VecVType::UINT32) ui = getUint32(buf, it->first);

			retval += to_string(ui);

		} else if (par->ixVType == Par::VecVType::BLOB) {
			if ((par->len > 32) && truncate) {
				if (truncated) *truncated = true;
				retval += "(" + to_string(par->len) + " bytes)";
			} else {
				retval += "0x";
				for (size_t j = 0; j < par->len; j++) retval += Dbe::binToHex(buf[it->first + j]);
			};

		} else if (par->ixVType == Par::VecVType::VBLOB) {
			if ((buf[it->first] > 32) && truncate) {
				if (truncated) *truncated = true;
				retval += "(" + to_string(buf[it->first]) + " bytes)";
			} else {
				retval += "0x";
				if (par->len > buf[it->first]) for (size_t j = 0; j < buf[it->first]; j++) retval += Dbe::binToHex(buf[it->first + j + 1]);
			};
		};
	};

	return retval;
};
