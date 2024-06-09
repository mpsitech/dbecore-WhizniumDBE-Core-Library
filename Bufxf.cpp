/**
	* \file Bufxf.cpp
	* buffer transfer functionality (implementation)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 23 Nov 2016
	*/

#include "Bufxf.h"

using namespace std;
using namespace Sbecore;

/******************************************************************************
 class bufxfref_t
 ******************************************************************************/

Dbecore::bufxfref_t::bufxfref_t(
			const uint32_t ixVState
			, const uint8_t rootTixWBuffer
			, const uint64_t bref
		) {
	this->ixVState = ixVState;
	this->rootTixWBuffer = rootTixWBuffer;
	this->bref = bref;
};

bool Dbecore::bufxfref_t::operator<(
			const bufxfref_t& comp
		) const {
	if (ixVState < comp.ixVState) return true;
	else if (ixVState > comp.ixVState) return false;

	if ((rootTixWBuffer == 0) || (comp.rootTixWBuffer == 0)) return false;
	if (rootTixWBuffer < comp.rootTixWBuffer) return true;
	else if (rootTixWBuffer > comp.rootTixWBuffer) return false;

	if ((bref == 0) || (comp.bref == 0)) return false;
	return(bref < comp.bref);
};

/******************************************************************************
 class bufxfref2_t
 ******************************************************************************/

Dbecore::bufxfref2_t::bufxfref2_t(
			const uint32_t ixVTarget
			, const uint64_t uref
			, const uint64_t bref
		) {
	this->ixVTarget = ixVTarget;
	this->uref = uref;
	this->bref = bref;
};

bool Dbecore::bufxfref2_t::operator<(
			const bufxfref2_t& comp
		) const {
	if (ixVTarget < comp.ixVTarget) return true;
	else if (ixVTarget > comp.ixVTarget) return false;

	if ((uref == 0) || (comp.uref == 0)) return false;
	if (uref < comp.uref) return true;
	else if (uref > comp.uref) return false;

	if ((bref == 0) || (comp.bref == 0)) return false;
	return(bref < comp.bref);
};

/******************************************************************************
 class Bufxf::VecVState
 ******************************************************************************/

uint32_t Dbecore::Bufxf::VecVState::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s == "void") return VOID;
	if (s == "waitprep") return WAITPREP;
	if (s == "waitinv") return WAITINV;
	if (s == "waitxfer") return WAITXFER;
	if (s == "waitret") return WAITRET;
	if (s == "done") return DONE;

	return 0;
};

string Dbecore::Bufxf::VecVState::getSref(
			const uint32_t ix
		) {
	if (ix == VOID) return("void");
	if (ix == WAITPREP) return("waitprep");
	if (ix == WAITINV) return("waitinv");
	if (ix == WAITXFER) return("waitxfer");
	if (ix == WAITRET) return("waitret");
	if (ix == DONE) return("done");

	return("");
};

string Dbecore::Bufxf::VecVState::getTitle(
			const uint32_t ix
		) {
	if (ix == VOID) return("invalid");
	if (ix == WAITPREP) return("wait for prepare");
	if (ix == WAITINV) return("wait for command invoke");
	if (ix == WAITXFER) return("wait for transfer");
	if (ix == WAITRET) return("wait for command return");
	if (ix == DONE) return("done");

	return("");
};

/******************************************************************************
 class Bufxf
 ******************************************************************************/

Dbecore::Bufxf::Bufxf(
			const uint8_t tixVBuffer
			, const bool writeNotRead
			, const size_t reqlen
			, const size_t prelen
			, const size_t postlen
			, unsigned char* buf
		) :
			cProgress("cProgress", "Bufxf", "Bufxf")
		{
	this->tixVBuffer = tixVBuffer;

	this->writeNotRead = writeNotRead;

	this->reqlen = reqlen;
	this->prelen = prelen;
	this->postlen = postlen;

	ixVTarget = 0;
	uref = 0;

	ixVState = VecVState::VOID;

	bref = 0;
	
	rootTixWBuffer = 0;

	dataExtNotInt = (buf != NULL);
	if (dataExtNotInt) data = buf;
	else data = new unsigned char[prelen+reqlen+postlen];

	ptr = prelen;

	success = false;

	progressCallback = NULL;
	argProgressCallback = NULL;

	errorCallback = NULL;
	argErrorCallback = NULL;

	doneCallback = NULL;
	argDoneCallback = NULL;
};

Dbecore::Bufxf::~Bufxf() {
	for (unsigned int i = 0; i < cmds.size(); i++) if (cmds[i]) delete cmds[i];

	if (!dataExtNotInt && data) delete[] data;
};

void Dbecore::Bufxf::appendReadData(
			const unsigned char* _data
			, const size_t _datalen
		) {
	size_t len = reqlen-ptr;
	if (_datalen < len) len = _datalen;

	memcpy(&(data[ptr]), _data, len);

	ptr += len;
};

unsigned char* Dbecore::Bufxf::getReadData() {
	unsigned char* retval = NULL;

	if (ptr > prelen) {
		retval = new unsigned char[ptr-prelen];
		memcpy(retval, &(data[prelen]), ptr-prelen);
	};

	return retval;
};

size_t Dbecore::Bufxf::getReadDatalen() {
	return(ptr-prelen);
};

void Dbecore::Bufxf::setWriteData(
			const unsigned char* _data
			, const size_t _datalen
		) {
	if (_datalen != reqlen) return;

	memcpy(&(data[prelen]), _data, _datalen);

	ptr = 0;
};

void Dbecore::Bufxf::setProgressCallback(
			bool (*_progressCallback)(Bufxf* bufxf, void* arg)
			, void* _argProgressCallback
		) {
	progressCallback = _progressCallback;
	argProgressCallback = _argProgressCallback;
};

void Dbecore::Bufxf::setErrorCallback(
			bool (*_errorCallback)(Bufxf* bufxf, void* arg)
			, void* _argErrorCallback
		) {
	errorCallback = _errorCallback;
	argErrorCallback = _argErrorCallback;
};

void Dbecore::Bufxf::setDoneCallback(
			bool (*_doneCallback)(Bufxf* bufxf, void* arg)
			, void* _argDoneCallback
		) {
	doneCallback = _doneCallback;
	argDoneCallback = _argDoneCallback;
};

void Dbecore::Bufxf::lockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.lockMutex(srefObject, srefMember, "bref=" + to_string(bref));
};

void Dbecore::Bufxf::signalProgress(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.signal(srefObject, srefMember, "bref=" + to_string(bref));
};

bool Dbecore::Bufxf::timedwaitProgress(
			const unsigned int dt
			, const string& srefObject
			, const string& srefMember
		) {
	return cProgress.timedwait(dt, srefObject, srefMember, "bref=" + to_string(bref));
};

void Dbecore::Bufxf::unlockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.unlockMutex(srefObject, srefMember, "bref=" + to_string(bref));
};
