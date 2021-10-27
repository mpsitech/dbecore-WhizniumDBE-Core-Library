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
 class cmdref_t
 ******************************************************************************/

Dbecore::cmdref_t::cmdref_t(
			const uint32_t ixVState
			, const uint32_t cref
		) {
	this->ixVState = ixVState;
	this->cref = cref;
};

bool Dbecore::cmdref_t::operator<(
			const cmdref_t& comp
		) const {
	if (ixVState < comp.ixVState) return true;
	else if (ixVState > comp.ixVState) return false;

	if ((cref == 0) || (comp.cref == 0)) return false;
	return(cref < comp.cref);
};

/******************************************************************************
 class cmdref2_t
 ******************************************************************************/

Dbecore::cmdref2_t::cmdref2_t(
			const uint32_t ixVTarget
			, const uint64_t uref
			, const uint32_t cref
		) {
	this->ixVTarget = ixVTarget;
	this->uref = uref;
	this->cref = cref;
};

bool Dbecore::cmdref2_t::operator<(
			const cmdref2_t& comp
		) const {
	if (ixVTarget < comp.ixVTarget) return true;
	else if (ixVTarget > comp.ixVTarget) return false;

	if ((uref == 0) || (comp.uref == 0)) return false;
	if (uref < comp.uref) return true;
	else if (uref > comp.uref) return false;

	if ((cref == 0) || (comp.cref == 0)) return false;
	return(cref < comp.cref);
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
 class Cmd::VecVState
 ******************************************************************************/

uint32_t Dbecore::Cmd::VecVState::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s == "void") return VOID;
	else if (s == "waitinv") return WAITINV;
	else if (s == "waitrev") return WAITREV;
	else if (s == "waitret") return WAITRET;
	else if (s == "waitnewret") return WAITNEWRET;
	else if (s == "done") return DONE;

	return 0;
};

string Dbecore::Cmd::VecVState::getSref(
			const uint32_t ix
		) {
	if (ix == VOID) return("void");
	else if (ix == WAITINV) return("waitinv");
	else if (ix == WAITREV) return("waitrev");
	else if (ix == WAITRET) return("waitret");
	else if (ix == WAITNEWRET) return("waitnewret");
	else if (ix == DONE) return("done");

	return("");
};

string Dbecore::Cmd::VecVState::getTitle(
			const uint32_t ix
		) {
	if (ix == VOID) return("invalid");
	else if (ix == WAITINV) return("wait for invoke");
	else if (ix == WAITREV) return("wait for revoke");
	else if (ix == WAITRET) return("wait for return");
	else if (ix == WAITNEWRET) return("wait for new return");
	else if (ix == DONE) return("done");

	return("");
};

/******************************************************************************
 class Cmd
 ******************************************************************************/

Dbecore::Cmd::Cmd(
			const uint8_t tixVCommand
			, const uint32_t ixVRettype
		) : Cmd(0x00, tixVCommand, ixVRettype) {
};

Dbecore::Cmd::Cmd(
			const uint8_t tixVController
			, const uint8_t tixVCommand
			, const uint32_t ixVRettype
		) :
			cProgress("cProgress", "Cmd", "Cmd")
		{
	this->tixVController = tixVController;
	this->tixVCommand = tixVCommand;

	this->ixVRettype = ixVRettype;

	ixVTarget = 0;
	uref = 0;

	ixVState = VecVState::VOID;

	cref = 0;
	
	Nret = 0;

	progressCallback = NULL;
	argProgressCallback = NULL;

	returnCallback = NULL;
	argReturnCallback = NULL;

	errorCallback = NULL;
	argErrorCallback = NULL;

	doneCallback = NULL;
	argDoneCallback = NULL;
};

Dbecore::Cmd::~Cmd() {
};

void Dbecore::Cmd::addParInv(
			const string& sref
			, const uint32_t ixVType
			, uint8_t (*getTixBySref)(const string& sref)
			, string (*getSrefByTix)(const uint8_t tix)
			, void (*fillFeed)(Feed& feed)
			, size_t buflen
		) {
	parsInv.insert(pair<string, Par>(sref, Par(sref, ixVType, getTixBySref, getSrefByTix, fillFeed, buflen)));
	seqParsInv.push_back(sref);
};

void Dbecore::Cmd::addParRet(
			const string& sref
			, const uint32_t ixVType
			, uint8_t (*getTixBySref)(const string& sref)
			, string (*getSrefByTix)(const uint8_t tix)
			, void (*fillFeed)(Feed& feed)
			, size_t buflen
		) {
	parsRet.insert(pair<string, Par>(sref, Par(sref, ixVType, getTixBySref, getSrefByTix, fillFeed, buflen)));
	seqParsRet.push_back(sref);
};

void Dbecore::Cmd::setProgressCallback(
			bool (*_progressCallback)(Cmd* cmd, void* arg)
			, void* _argProgressCallback
		) {
	progressCallback = _progressCallback;
	argProgressCallback = _argProgressCallback;
};

void Dbecore::Cmd::setReturnCallback(
			void (*_returnCallback)(Cmd* cmd, void* arg)
			, void* _argReturnCallback
		) {
	returnCallback = _returnCallback;
	argReturnCallback = _argReturnCallback;
};

void Dbecore::Cmd::setErrorCallback(
			bool (*_errorCallback)(Cmd* cmd, void* arg)
			, void* _argErrorCallback
		) {
	errorCallback = _errorCallback;
	argErrorCallback = _argErrorCallback;
};

void Dbecore::Cmd::setDoneCallback(
			bool (*_doneCallback)(Cmd* cmd, void* arg)
			, void* _argDoneCallback
		) {
	doneCallback = _doneCallback;
	argDoneCallback = _argDoneCallback;
};

void Dbecore::Cmd::returnToCallback() {
	if (returnCallback) returnCallback(this, argReturnCallback);
};

void Dbecore::Cmd::lockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.lockMutex(srefObject, srefMember, "cref=" + to_string(cref));
};

void Dbecore::Cmd::signalProgress(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.signal(srefObject, srefMember, "cref=" + to_string(cref));
};

void Dbecore::Cmd::waitProgress(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.wait(srefObject, srefMember, "cref=" + to_string(cref));
};

bool Dbecore::Cmd::timedwaitProgress(
			const unsigned int dt
			, const string& srefObject
			, const string& srefMember
		) {
	return cProgress.timedwait(dt, srefObject, srefMember, "cref=" + to_string(cref));
};

void Dbecore::Cmd::unlockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.lockMutex(srefObject, srefMember, "cref=" + to_string(cref));
};

string Dbecore::Cmd::parsToTemplate(
			const bool retNotInv
		) {
	string retval;

	map<string, Par>& pars = [&]() ->map<string, Par>& {if (!retNotInv) return parsInv; return parsRet;}();
	vector<string>& seqPars = [&]() ->vector<string>& {if (!retNotInv) return seqParsInv; return seqParsRet;}();

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

			} else if (par->ixVType == Par::VecVType::BLOB) retval += "[blob" + to_string(par->buflen) + "]";
			else if (par->ixVType == Par::VecVType::VBLOB) retval += "[len,vblob" + to_string(par->buflen) + "]";
			else retval += "[" + Par::VecVType::getSref(par->ixVType) + "]";
		};
	};

	return retval;
};

void Dbecore::Cmd::hexToParsInv(
			const string& s
		) {
	// all invocation parameters will be reset and overwritten with as much data as is available in s

	unsigned char* buf = NULL;
	size_t buflen;

	Par* par = NULL;

	size_t bufptr;

	Dbe::hexToBuf(s, &buf, buflen);

	bufptr = 0;

	for (unsigned int i = 0; i < seqParsInv.size(); i++) {
		auto it = parsInv.find(seqParsInv[i]);

		if (it != parsInv.end()) {
			par = &(it->second);

			par->reset();

			if (bufptr < buflen) {
				if (par->ixVType == Par::VecVType::VBLOB) {
					if ((buflen-bufptr) >= buf[bufptr]) {
						par->setVblob(&(buf[bufptr+1]), buf[bufptr]);
						bufptr += par->buflen;
					} else {
						par->setVblob(&(buf[bufptr+1]), buflen-bufptr-1);
						bufptr = buflen;
					};

				} else if (par->buf) {
					if ((buflen-bufptr) >= par->buflen) {
						memcpy(par->buf, &(buf[bufptr]), par->buflen);
						bufptr += par->buflen;
					} else {
						memcpy(par->buf, &(buf[bufptr]), buflen-bufptr);
						bufptr = buflen;
					};
				};
			};
		};
	};

	if (buf) delete[] buf;
};

void Dbecore::Cmd::parlistToParsInv(
			const string& s
		) {
	// example for s: "par1=0x00ef,par2=-123,par3=false;"
	// only the invocation parameters found in s will be overwritten

	vector<string> ss;

	string key, val;
	size_t ptr;

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
				auto it = parsInv.find(key);

				if (it != parsInv.end()) {
					par = &(it->second);

					if (par->ixVType == Par::VecVType::TIX) {
						if (par->getTixBySref) par->setTix(par->getTixBySref(val));

					} else if (par->ixVType == Par::VecVType::_BOOL) {
						par->setBool(StrMod::lc(val) == "true");

					} else if ((par->ixVType == Par::VecVType::INT8) || (par->ixVType == Par::VecVType::UINT8) || (par->ixVType == Par::VecVType::INT16)
								|| (par->ixVType == Par::VecVType::UINT16) || (par->ixVType == Par::VecVType::INT32) || (par->ixVType == Par::VecVType::UINT32)) {

						// number or hex code
						if (val.find("0x") == 0) {
							Dbe::hexToBuf(val, &buf, buflen);

							if ((par->ixVType == Par::VecVType::INT8) || (par->ixVType == Par::VecVType::UINT8)) {
								if (buflen == 1) memcpy(par->buf, buf, 1); else memset(par->buf, 0, 1);
							} else if ((par->ixVType == Par::VecVType::INT16) || (par->ixVType == Par::VecVType::UINT16)) {
								if (buflen == 2) memcpy(par->buf, buf, 2); else memset(par->buf, 0, 2);
							} else if ((par->ixVType == Par::VecVType::INT32) || (par->ixVType == Par::VecVType::UINT32)) {
								if (buflen == 4) memcpy(par->buf, buf, 4); else memset(par->buf, 0, 4);
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
									par->setInt8((int8_t) intval);
									break;
								case Par::VecVType::UINT8:
									par->setUint8((uint8_t) intval);
									break;
								case Par::VecVType::INT16:
									par->setInt16((int16_t) intval);
									break;
								case Par::VecVType::UINT16:
									par->setUint16((uint16_t) intval);
									break;
								case Par::VecVType::INT32:
									par->setInt32((int32_t) intval);
									break;
								case Par::VecVType::UINT32:
									par->setUint32((uint32_t) intval);
									break;
							};
						};

					} else if ((par->ixVType == Par::VecVType::BLOB) || (par->ixVType == Par::VecVType::VBLOB)) {
						// expect hex code
						Dbe::hexToBuf(val, &buf, buflen);

						if (par->ixVType == Par::VecVType::BLOB) {
							par->setBlob(buf, buflen);

						} else if (par->ixVType == Par::VecVType::VBLOB) {
							if ((buflen-1) < buf[0]) par->setVblob(&(buf[1]), buflen-1);
							else par->setVblob(&(buf[1]), buf[0]);
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

void Dbecore::Cmd::parsInvToBuf(
			unsigned char** buf
			, size_t& buflen
		) {
	Par::parsToBuf(parsInv, seqParsInv, buf, buflen);
};

size_t Dbecore::Cmd::getInvBuflen() {
	size_t buflen = 0;

	for (auto it = parsInv.begin(); it != parsInv.end(); it++) buflen += it->second.buflen;

	return buflen;
};

string Dbecore::Cmd::getInvText(
			const bool truncate
			, bool* truncated
		) {
	return Par::parsToText(parsInv, seqParsInv, truncate, truncated);
};

string Dbecore::Cmd::getInvHex(
			const bool truncate
			, bool* truncated
		) {
	string hex;

	unsigned char* buf = NULL;
	size_t buflen;

	hex = "0x" + Dbe::binToHex(tixVController) + Dbe::binToHex(tixVCommand);

	parsInvToBuf(&buf, buflen);

	if (buf) {
		hex += Dbe::bufToHex(buf, buflen, truncate, truncated);
		delete[] buf;
	};

	return hex;
};

void Dbecore::Cmd::resetParsRet() {
	for (auto it = parsRet.begin(); it != parsRet.end(); it++) it->second.reset();
};

void Dbecore::Cmd::bufToParsRet(
			const unsigned char* buf
			, const size_t buflen
		) {
	// all return parameters will be reset and overwritten with as much data as is available in buf

	Par* par = NULL;

	size_t bufptr;

	bufptr = 0;

	for (unsigned int i = 0; i < seqParsRet.size(); i++) {
		auto it = parsRet.find(seqParsRet[i]);

		if (it != parsRet.end()) {
			par = &(it->second);

			par->reset();

			if (bufptr < buflen) {
				if (par->ixVType == Par::VecVType::VBLOB) {
					if ((buflen-bufptr) >= buf[bufptr]) {
						par->setVblob(&(buf[bufptr+1]), buf[bufptr]);
						bufptr += par->buflen;
					} else {
						par->setVblob(&(buf[bufptr+1]), buflen-bufptr-1);
						bufptr = buflen;
					};

				} else if (par->buf) {
					if ((buflen-bufptr) >= par->buflen) {
						memcpy(par->buf, &(buf[bufptr]), par->buflen);
						bufptr += par->buflen;
					} else {
						memcpy(par->buf, &(buf[bufptr]), buflen-bufptr);
						bufptr = buflen;
					};
				};
			};
		};
	};
};

void Dbecore::Cmd::parsRetToBuf(
			unsigned char** buf
			, size_t& buflen
		) {
	Par::parsToBuf(parsRet, seqParsRet, buf, buflen);
};

size_t Dbecore::Cmd::getRetBuflen() {
	size_t buflen = 0;

	for (auto it = parsRet.begin(); it != parsRet.end(); it++) buflen += it->second.buflen;

	return buflen;
};

string Dbecore::Cmd::getRetText(
			const bool truncate
			, bool* truncated
		) {
	return Par::parsToText(parsRet, seqParsRet, truncate, truncated);
};

string Dbecore::Cmd::getRetHex(
			const bool truncate
			, bool* truncated
		) {
	string hex;

	unsigned char* buf = NULL;
	size_t buflen;

	hex = "0x";

	parsRetToBuf(&buf, buflen);

	if (buf) {
		hex += Dbe::bufToHex(buf, buflen, truncate, truncated);
		delete[] buf;
	};

	return hex;
};
