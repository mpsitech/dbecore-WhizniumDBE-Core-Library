/**
	* \file Cmd.cpp
	* command functionality (implementation)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 3 Feb 2016
	*/

#include "Cmd.h"

using namespace std;
using namespace Sbecore;
using namespace Xmlio;

/******************************************************************************
 class cmdix_t
 ******************************************************************************/

Dbecore::cmdix_t::cmdix_t(
			const uint32_t ixVTarget
			, const uint8_t tixVController
			, const uint8_t tixVCommand
		) {
	this->ixVTarget = ixVTarget;
	this->tixVController = tixVController;
	this->tixVCommand = tixVCommand;
};

bool Dbecore::cmdix_t::operator<(
			const cmdix_t& comp
		) const {
	if (ixVTarget < comp.ixVTarget) return true;
	else if (ixVTarget > comp.ixVTarget) return false;

	if ((tixVController == 0) || (comp.tixVController == 0)) return false;
	if (tixVController < comp.tixVController) return true;
	else if (tixVController > comp.tixVController) return false;

	if ((tixVCommand == 0) || (comp.tixVCommand == 0)) return false;
	return(tixVCommand < comp.tixVCommand);
};

/******************************************************************************
 class Cmd::VecVRettype
 ******************************************************************************/

uint32_t Dbecore::Cmd::VecVRettype::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s == "void") return VOID;
	else if (s == "statsng") return STATSNG;
	else if (s == "immsng") return IMMSNG;
	else if (s == "dfrsng") return DFRSNG;
	else if (s == "mult") return MULT;

	return 0;
};

string Dbecore::Cmd::VecVRettype::getSref(
			const uint32_t ix
		) {
	if (ix == VOID) return("void");
	else if (ix == STATSNG) return("statsng");
	else if (ix == IMMSNG) return("immsng");
	else if (ix == DFRSNG) return("dfrsng");
	else if (ix == MULT) return("mult");

	return("");
};

string Dbecore::Cmd::VecVRettype::getTitle(
			const uint32_t ix
		) {
	if (ix == VOID) return("none");
	else if (ix == STATSNG) return("static single");
	else if (ix == IMMSNG) return("immediate single");
	else if (ix == DFRSNG) return("deferred single");
	else if (ix == MULT) return("multiple");

	return("");
};

/******************************************************************************
 class Cmd
 ******************************************************************************/

Dbecore::Cmd::Cmd(
			const uint8_t tixVCommand
			, const uint32_t ixVRettype
			, uint32_t (*invparGetIxBySref)(const string& sref)
			, string (*invparGetSrefByIx)(const uint32_t ix)
			, uint32_t (*retparGetIxBySref)(const string& sref)
			, string (*retparGetSrefByIx)(const uint32_t ix)
		) : Cmd(0x00, tixVCommand, ixVRettype, invparGetIxBySref, invparGetSrefByIx, retparGetIxBySref, retparGetSrefByIx) {
};

Dbecore::Cmd::Cmd(
			const uint8_t tixVController
			, const uint8_t tixVCommand
			, const uint32_t ixVRettype
			, uint32_t (*invparGetIxBySref)(const string& sref)
			, string (*invparGetSrefByIx)(const uint32_t ix)
			, uint32_t (*retparGetIxBySref)(const string& sref)
			, string (*retparGetSrefByIx)(const uint32_t ix)
		) :
			tixVController(tixVController)
			, tixVCommand(tixVCommand)
			, ixVRettype(ixVRettype)
			, invparGetIxBySref(invparGetIxBySref)
			, invparGetSrefByIx(invparGetSrefByIx)
			, retparGetIxBySref(retparGetIxBySref)
			, retparGetSrefByIx(retparGetSrefByIx)
		{
	lenParbufInv = 0;
	parbufInv = NULL;

	lenParbufRet = 0;
	parbufRet = NULL;

	fixeddone = false;
};

Dbecore::Cmd::~Cmd() {
	if (parbufInv) delete[] parbufInv;
	if (parbufRet) delete[] parbufRet;
};

void Dbecore::Cmd::addInvpar(
			const uint32_t ix
			, const uint32_t ixVType
			, uint8_t (*getTixBySref)(const string& sref)
			, string (*getSrefByTix)(const uint8_t tix)
			, void (*fillFeed)(Feed& feed)
			, size_t len
		) {
	invpars.insert(pair<uint32_t, Par>(ix, Par(invparGetSrefByIx(ix), ixVType, getTixBySref, getSrefByTix, fillFeed, len)));
};

void Dbecore::Cmd::addRetpar(
			const uint32_t ix
			, const uint32_t ixVType
			, uint8_t (*getTixBySref)(const string& sref)
			, string (*getSrefByTix)(const uint8_t tix)
			, void (*fillFeed)(Feed& feed)
			, size_t len
		) {
	retpars.insert(pair<uint32_t, Par>(ix, Par(retparGetSrefByIx(ix), ixVType, getTixBySref, getSrefByTix, fillFeed, len)));
};

void Dbecore::Cmd::composeFixed() {
	Crc crc(0x8005, false);

	lenParbufInv = 0;
	for (auto it = invpars.begin(); it != invpars.end(); it++) lenParbufInv += it->second.len;
	if (lenParbufInv != 0) parbufInv = new uint8_t[lenParbufInv + 2];

	lenParbufRet = 0;
	for (auto it = retpars.begin(); it != retpars.end(); it++) lenParbufRet += it->second.len;
	if (lenParbufRet != 0) parbufRet = new uint8_t[lenParbufRet + 2];

	// tx1
	tx1[0] = 0x01; // tixVBuffer
	tx1[1] = tixVController;
	tx1[2] = tixVCommand;
	tx1[3] = (lenParbufInv >> 24) & 0x000000FF; // length
	tx1[4] = (lenParbufInv >> 16) & 0x000000FF;
	tx1[5] = (lenParbufInv >> 8) & 0x000000FF;
	tx1[6] = lenParbufInv & 0x000000FF;

	crc.reset();
	crc.includeBytes(tx1, 7);
	crc.finalize();
	tx1[7] = (crc.crc >> 8) & 0x00FF;
	tx1[8] = crc.crc & 0x00FF;

	// tx3
	tx3[0] = 0x00; // tixVBuffer = cmdretToHostif
	tx3[1] = 0x00; // tixVController
	tx3[2] = VecDbeVBufxfop::POLL;
	tx3[3] = 0x00; // length = 4
	tx3[4] = 0x00;
	tx3[5] = 0x00;
	tx3[6] = 0x04;

	crc.reset();
	crc.includeBytes(tx3, 7);
	crc.finalize();
	tx3[7] = (crc.crc >> 8) & 0x00FF;
	tx3[8] = crc.crc & 0x00FF;

	// rx4
	rx4[0] = tixVController;
	rx4[1] = tixVCommand;
	rx4[2] = (lenParbufRet >> 8) & 0x000000FF;
	rx4[3] = lenParbufRet & 0x000000FF;

	crc.reset();
	crc.includeBytes(rx4, 4);
	crc.finalize();
	rx4[4] = (crc.crc >> 8) & 0x00FF;
	rx4[5] = crc.crc & 0x00FF;

	fixeddone = true;
};

string Dbecore::Cmd::parsToTemplate(
			const bool retNotInv
		) {
	string retval;

	map<uint32_t, Par>& pars = [&]() ->map<uint32_t, Par>& {if (!retNotInv) return invpars; return retpars;}();

	Par* par = NULL;

	Feed feed;

	bool first;

	first = true;

	for (auto it = pars.begin(); it != pars.end(); it++) {
		par = &(it->second);

		if (first) first = false;
		else retval += ",";

		retval += par->sref;
		if (!retNotInv) retval += "=";

		if (par->ixVType == Par::VecVType::_BOOL) retval += "{false,true}";
		else if (par->ixVType == Par::VecVType::TIX) {
			retval += "{";

			if (par->fillFeed) {
				par->fillFeed(feed);

				for (unsigned int j = 0; j < feed.size(); j++) {
					if (j != 0) retval += ",";
					retval += feed.getSrefByNum(j+1);
				};
			};

			retval += "}";

		} else if (par->ixVType == Par::VecVType::BLOB) retval += "[blob" + to_string(par->len) + "]";
		else if (par->ixVType == Par::VecVType::VBLOB) retval += "[len,vblob" + to_string(par->len-1) + "]";
		else retval += "[" + Par::VecVType::getSref(par->ixVType) + "]";
	};

	return retval;
};

string Dbecore::Cmd::getParsText(
			const bool retNotInv
			, const bool truncate
			, bool* truncated
		) {
	if (!retNotInv) return Par::parsToText(invpars, parbufInv, truncate, truncated);
	return Par::parsToText(retpars, parbufRet, truncate, truncated);
};

string Dbecore::Cmd::getParsHex(
			const bool retNotInv
			, const bool truncate
			, bool* truncated
		) {
	string hex;

	hex = "0x";
	if (!retNotInv && parbufInv) hex += Dbe::bufToHex(parbufInv, lenParbufInv, truncate, truncated);
	else if (retNotInv && parbufRet) hex += Dbe::bufToHex(parbufRet, lenParbufRet, truncate, truncated);

	return hex;
};

void Dbecore::Cmd::resetParbufInv() {
	memset(parbufInv, 0, lenParbufInv);
};

void Dbecore::Cmd::hexToParbufInv(
			const string& s
		) {
	resetParbufInv();

	for (unsigned int i = 0; (i < s.size()/2) && (i < lenParbufInv); i++) parbufInv[i] = Dbe::hexToBin(s.substr(2*i, 2));
};

void Dbecore::Cmd::parlistToParbufInv(
			const string& s
		) {
	// example for s: "par1=0x00ef,par2=-123,par3=false;"
	// only the invocation parameters found in s will be overwritten

	vector<string> ss;

	string key, val;
	size_t ptr;

	size_t ofs;
	Par* par = NULL;

	unsigned char* buf = NULL;
	size_t buflen;

	int intval;

	StrMod::stringToVector(s, ss, ',', true);

	for (unsigned int i = 0; i < ss.size(); i++) {
		ptr = ss[i].find('=');

		if (ptr != string::npos) {
			key = ss[i].substr(0, ptr);
			val = ss[i].substr(ptr+1);

			if ((key != "") && (val != "")) {
				ofs = invparGetIxBySref(key);
				auto it = invpars.find(ofs);

				if (it != invpars.end()) {
					par = &(it->second);

					if (par->ixVType == Par::VecVType::TIX) {
						if (par->getTixBySref) Par::setTix(parbufInv, ofs, par->getTixBySref(val));

					} else if (par->ixVType == Par::VecVType::_BOOL) {
						Par::setBool(parbufInv, ofs, StrMod::lc(val) == "true");

					} else if ((par->ixVType == Par::VecVType::INT8) || (par->ixVType == Par::VecVType::UINT8) || (par->ixVType == Par::VecVType::INT16)
								|| (par->ixVType == Par::VecVType::UINT16) || (par->ixVType == Par::VecVType::INT32) || (par->ixVType == Par::VecVType::UINT32)) {

						// number or hex code
						if (val.find("0x") == 0) {
							Dbe::hexToBuf(val, &buf, buflen);

							if ((par->ixVType == Par::VecVType::INT8) || (par->ixVType == Par::VecVType::UINT8)) {
								if (buflen == 1) memcpy(&(parbufInv[ofs]), buf, 1); else memset(&(parbufInv[ofs]), 0, 1);
							} else if ((par->ixVType == Par::VecVType::INT16) || (par->ixVType == Par::VecVType::UINT16)) {
								if (buflen == 2) memcpy(&(parbufInv[ofs]), buf, 2); else memset(&(parbufInv[ofs]), 0, 2);
							} else if ((par->ixVType == Par::VecVType::INT32) || (par->ixVType == Par::VecVType::UINT32)) {
								if (buflen == 4) memcpy(&(parbufInv[ofs]), buf, 4); else memset(&(parbufInv[ofs]), 0, 4);
							};

							if (buf) {
								delete[] buf;
								buf = NULL;
								
								buflen = 0;
							};

						} else {
							intval = atoi(val.c_str());

							switch (par->ixVType) {
								case Par::VecVType::INT8:
									Par::setInt8(parbufInv, ofs, (int8_t) intval);
									break;
								case Par::VecVType::UINT8:
									Par::setUint8(parbufInv, ofs, (uint8_t) intval);
									break;
								case Par::VecVType::INT16:
									Par::setInt16(parbufInv, ofs, (int16_t) intval);
									break;
								case Par::VecVType::UINT16:
									Par::setUint16(parbufInv, ofs, (uint16_t) intval);
									break;
								case Par::VecVType::INT32:
									Par::setInt32(parbufInv, ofs, (int32_t) intval);
									break;
								case Par::VecVType::UINT32:
									Par::setUint32(parbufInv, ofs, (uint32_t) intval);
									break;
							};
						};

					} else if ((par->ixVType == Par::VecVType::BLOB) || (par->ixVType == Par::VecVType::VBLOB)) {
						// expect hex code
						Dbe::hexToBuf(val, &buf, buflen);

						if (par->ixVType == Par::VecVType::BLOB) {
							Par::setBlob(parbufInv, ofs, par->len, buf, buflen);

						} else if (par->ixVType == Par::VecVType::VBLOB) {
							if ((buflen-1) < buf[0]) Par::setVblob(parbufInv, ofs, par->len, &(buf[1]), buflen-1);
							else Par::setVblob(parbufInv, ofs, par->len, &(buf[1]), buf[0]);
						};

						if (buf) {
							delete[] buf;
							buf = NULL;
							
							buflen = 0;
						};
					};
				};
			};
		};
	};
};

void Dbecore::Cmd::resetParbufRet() {
	memset(parbufRet, 0, lenParbufRet);
};
