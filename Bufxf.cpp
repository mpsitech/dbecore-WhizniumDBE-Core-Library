/**
  * \file Bufxf.cpp
  * buffer transfer functionality (implementation)
  * \author Alexander Wirthmüller
  * \date created: 23 Nov 2016
  * \date modified: 29 Apr 2017
  */

#include "Bufxf.h"

/******************************************************************************
 class bufxfref_t
 ******************************************************************************/

bufxfref_t::bufxfref_t(
			const uint ixVState
			, const utinyint rootTixWBuffer
			, const ubigint bref
		) {
	this->ixVState = ixVState;
	this->rootTixWBuffer = rootTixWBuffer;
	this->bref = bref;
};

bool bufxfref_t::operator<(
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

bufxfref2_t::bufxfref2_t(
			const uint ixVTarget
			, const ubigint uref
			, const ubigint bref
		) {
	this->ixVTarget = ixVTarget;
	this->uref = uref;
	this->bref = bref;
};

bool bufxfref2_t::operator<(
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

uint Bufxf::VecVState::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s == "void") return VOID;
	else if (s == "waitprep") return WAITPREP;
	else if (s == "waitinv") return WAITINV;
	else if (s == "waitxfer") return WAITXFER;
	else if (s == "waitret") return WAITRET;
	else if (s == "done") return DONE;

	return 0;
};

string Bufxf::VecVState::getSref(
			const uint ix
		) {
	if (ix == VOID) return("void");
	else if (ix == WAITPREP) return("waitprep");
	else if (ix == WAITINV) return("waitinv");
	else if (ix == WAITXFER) return("waitxfer");
	else if (ix == WAITRET) return("waitret");
	else if (ix == DONE) return("done");

	return("");
};

string Bufxf::VecVState::getTitle(
			const uint ix
		) {
	if (ix == VOID) return("invalid");
	else if (ix == WAITPREP) return("wait for prepare");
	else if (ix == WAITINV) return("wait for command invoke");
	else if (ix == WAITXFER) return("wait for transfer");
	else if (ix == WAITRET) return("wait for command return");
	else if (ix == DONE) return("done");

	return("");
};

/******************************************************************************
 class Bufxf
 ******************************************************************************/

Bufxf::Bufxf(
			const utinyint tixWBuffer
			, const bool writeNotRead
			, const size_t reqlen
			, const size_t prelen
			, const size_t postlen
			, unsigned char* buf
		) :
			cProgress("cProgress", "Bufxf", "Bufxf")
		{
	this->tixWBuffer = tixWBuffer;

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

Bufxf::~Bufxf() {
	for (unsigned int i = 0; i < cmds.size(); i++) if (cmds[i]) delete cmds[i];

	if (!dataExtNotInt && data) delete[] data;
};

void Bufxf::appendReadData(
			const unsigned char* _data
			, const size_t _datalen
		) {
	size_t len = reqlen-ptr;
	if (_datalen < len) len = _datalen;

	memcpy(&(data[ptr]), _data, len);

	ptr += len;
};

unsigned char* Bufxf::getReadData() {
	unsigned char* retval = NULL;

	if (ptr > prelen) {
		retval = new unsigned char[ptr-prelen];
		memcpy(retval, &(data[prelen]), ptr-prelen);
	};

	return retval;
};

size_t Bufxf::getReadDatalen() {
	return(ptr-prelen);
};

void Bufxf::setWriteData(
			const unsigned char* _data
			, const size_t _datalen
		) {
	if (_datalen != reqlen) return;

	memcpy(&(data[prelen]), _data, _datalen);

	ptr = 0;
};

void Bufxf::setProgressCallback(
			bool (*_progressCallback)(Bufxf* bufxf, void* arg)
			, void* _argProgressCallback
		) {
	progressCallback = _progressCallback;
	argProgressCallback = _argProgressCallback;
};

void Bufxf::setErrorCallback(
			bool (*_errorCallback)(Bufxf* bufxf, void* arg)
			, void* _argErrorCallback
		) {
	errorCallback = _errorCallback;
	argErrorCallback = _argErrorCallback;
};

void Bufxf::setDoneCallback(
			bool (*_doneCallback)(Bufxf* bufxf, void* arg)
			, void* _argDoneCallback
		) {
	doneCallback = _doneCallback;
	argDoneCallback = _argDoneCallback;
};

void Bufxf::lockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.lockMutex(srefObject, srefMember);
};

void Bufxf::signalProgress(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.signal(srefObject, srefMember);
};

bool Bufxf::timedwaitProgress(
			const unsigned int dt
			, const string& srefObject
			, const string& srefMember
		) {
	return cProgress.timedwait(dt, srefObject, srefMember);
};

void Bufxf::unlockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.unlockMutex(srefObject, srefMember);
};
