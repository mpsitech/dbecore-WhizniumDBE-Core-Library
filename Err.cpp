/**
  * \file Err.cpp
  * error functionality (implementation)
  * \author Alexander Wirthmüller
  * \date created: 29 Apr 2017
  * \date modified: 22 Apr 2020
  */

#include "Err.h"

using namespace std;
using namespace Sbecore;
using namespace Xmlio;

/******************************************************************************
 class Err
 ******************************************************************************/

Dbecore::Err::Err(
			const utinyint tixDbeVAction
			, const utinyint tixVError
		) {
	this->tixDbeVAction = tixDbeVAction;
	this->tixVError = tixVError;
};

Dbecore::Err::~Err() {
};

void Dbecore::Err::addPar(
			const string& sref
			, const uint ixVType
			, utinyint (*getTixBySref)(const string& sref)
			, string (*getSrefByTix)(const utinyint tix)
			, void (*fillFeed)(Feed& feed)
			, size_t buflen
		) {
	pars.insert(pair<string,Par>(sref, Par(sref, ixVType, getTixBySref, getSrefByTix, fillFeed, buflen))); seqPars.push_back(sref);
};

void Dbecore::Err::bufToPars(
			const unsigned char* buf
			, const size_t buflen
		) {
	// all parameters will be reset and overwritten with as much data as is available in buf

	Par* par = NULL;

	size_t bufptr;

	bufptr = 0;

	for (unsigned int i = 0; i < seqPars.size(); i++) {
		auto it = pars.find(seqPars[i]);

		if (it != pars.end()) {
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

void Dbecore::Err::parsToBuf(
			unsigned char** buf
			, size_t& buflen
		) {
	Par::parsToBuf(pars, seqPars, buf, buflen);
};

size_t Dbecore::Err::getBuflen() {
	size_t buflen = 0;

	for (auto it = pars.begin(); it != pars.end(); it++) buflen += it->second.buflen;

	return buflen;
};

string Dbecore::Err::getParText(
			const bool truncate
			, bool* truncated
		) {
	return Par::parsToText(pars, seqPars, truncate, truncated);
};

string Dbecore::Err::getParHex(
			const bool truncate
			, bool* truncated
		) {
	string hex;

	unsigned char* buf = NULL;
	size_t buflen;

	hex = "0x";

	parsToBuf(&buf, buflen);

	if (buf) {
		hex += Dbe::bufToHex(buf, buflen, truncate, truncated);
		delete[] buf;
	};

	return hex;
};

string Dbecore::Err::getMessage(
			const string& srefCtr
			, const string& srefCmd
			, const uint cref
			, const string& srefErr
			, const string& titErr
			, const bool cmdNotErronly
			, const bool titleNotSref
		) {
	// ex. 1: error running command alua.add(4) - invalid command: cmderr
	// ex. 2: error running command alua.add(4) - conflicting cref on invoke/revoke: creferr(cref=123)
	// ex. 3: error running command alua.add(4) - invalid register for operand: alua.invalid(invalidA=true,invalidB=false)

	// ex. 3, cmdNotErronly=false, titleNotSref=false: alua.invalid(invalidA=true,invalidB=false)
	// ex. 3, cmdNotErronly=false, titleNotSref=true: invalid register for operand: alua.invalid(invalidA=true,invalidB=false)
	// ex. 3, cmdNotErronly=true, titleNotSref=false: alua.add(4) error: alua.invalid(invalidA=true,invalidB=false)

	string msg;
	string s;

	if (tixDbeVAction == 0x00) {
		if (titleNotSref) msg = "no error";
		else msg = "ok";
	
	} else {
		if (cmdNotErronly) {
			msg = srefCtr;
			if (msg != "") msg += ".";
			msg += srefCmd + "(" + to_string(cref) + ")";

			if (titleNotSref) msg = "error running command " + msg + " - ";
			else msg += " error: ";
		};

		if (titleNotSref) {
			if (tixDbeVAction == VecDbeVAction::ERR) s = titErr;
			else s = VecDbeVAction::getTitle(tixDbeVAction);
			if (s == "") s = "unknown error";
			
			msg += s + ": ";
		};

		if (tixDbeVAction == VecDbeVAction::ERR) s = srefErr;
		else s = VecDbeVAction::getSref(tixDbeVAction);
		if (s == "") s = "unknown";

		if ((tixDbeVAction == VecDbeVAction::ERR) && (srefCtr != "")) s = srefCtr + "." + s;

		msg += s;

		s = getParText();
		if (s != "") s = "(" + s + ")";
		
		msg += s;
	};

	return msg;
};

Dbecore::Err Dbecore::Err::getNewRteerr() {
	return Err(VecDbeVAction::RTEERR);
};

Dbecore::Err Dbecore::Err::getNewCreferr() {
	return Err(VecDbeVAction::CREFERR);
};

Dbecore::Err Dbecore::Err::getNewFwderr() {
	Err err(VecDbeVAction::FWDERR);

	err.addPar("routeLocn", Par::VecVType::BLOB, NULL, NULL, NULL, 4);

	return err;
};

Dbecore::Err Dbecore::Err::getNewCmderr() {
	return Err(VecDbeVAction::CMDERR);
};

Dbecore::Err Dbecore::Err::getNewToerr() {
	return Err(VecDbeVAction::TOERR);
};

Dbecore::Err Dbecore::Err::getNewRsterr() {
	return Err(VecDbeVAction::RSTERR);
};
