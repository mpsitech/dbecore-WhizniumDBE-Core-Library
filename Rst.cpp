/**
	* \file Rst.cpp
	* reset functionality (implementation)
	* \copyright (C) 2017-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 29 Apr 2017
	*/

#include "Rst.h"

using namespace std;
using namespace Sbecore;

/******************************************************************************
 class rstref_t
 ******************************************************************************/

Dbecore::rstref_t::rstref_t(
			const uint ixVState
			, const ubigint rref
		) {
	this->ixVState = ixVState;
	this->rref = rref;
};

bool Dbecore::rstref_t::operator<(
			const rstref_t& comp
		) const {
	if (ixVState < comp.ixVState) return true;
	else if (ixVState > comp.ixVState) return false;

	if ((rref == 0) || (comp.rref == 0)) return false;
	return(rref < comp.rref);
};

/******************************************************************************
 class rstref2_t
 ******************************************************************************/

Dbecore::rstref2_t::rstref2_t(
			const uint ixVTarget
			, const ubigint uref
			, const ubigint rref
		) {
	this->ixVTarget = ixVTarget;
	this->uref = uref;
	this->rref = rref;
};

bool Dbecore::rstref2_t::operator<(
			const rstref2_t& comp
		) const {
	if (ixVTarget < comp.ixVTarget) return true;
	else if (ixVTarget > comp.ixVTarget) return false;

	if ((uref == 0) || (comp.uref == 0)) return false;
	if (uref < comp.uref) return true;
	else if (uref > comp.uref) return false;

	if ((rref == 0) || (comp.rref == 0)) return false;
	return(rref < comp.rref);
};

/******************************************************************************
 class Rst::VecVState
 ******************************************************************************/

uint Dbecore::Rst::VecVState::getIx(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s == "void") return VOID;
	else if (s == "waitprep") return WAITPREP;
	else if (s == "waitinv") return WAITINV;
	else if (s == "waitrst") return WAITRST;
	else if (s == "done") return DONE;

	return 0;
};

string Dbecore::Rst::VecVState::getSref(
			const uint ix
		) {
	if (ix == VOID) return("void");
	else if (ix == WAITPREP) return("waitprep");
	else if (ix == WAITINV) return("waitinv");
	else if (ix == WAITRST) return("waitrst");
	else if (ix == DONE) return("done");

	return("");
};

string Dbecore::Rst::VecVState::getTitle(
			const uint ix
		) {
	if (ix == VOID) return("invalid");
	else if (ix == WAITPREP) return("wait for prepare");
	else if (ix == WAITINV) return("wait for command invoke (non-root)");
	else if (ix == WAITRST) return("wait for reset (root or non-root)");
	else if (ix == DONE) return("done");

	return("");
};

/******************************************************************************
 class Rst
 ******************************************************************************/

Dbecore::Rst::Rst(
			const uint ixVTarget
			, const ubigint uref
		) :
			cProgress("cProgress", "Rst", "Rst")
		{
	this->ixVTarget = ixVTarget;
	this->uref = uref;

	ixVState = VecVState::VOID;

	root = false;
	cmd = NULL;

	progressCallback = NULL;
	argProgressCallback = NULL;
};

Dbecore::Rst::~Rst() {
	if (cmd) delete cmd;
};

void Dbecore::Rst::setProgressCallback(
			bool (*_progressCallback)(Rst* rst, void* arg)
			, void* _argProgressCallback
		) {
	progressCallback = _progressCallback;
	argProgressCallback = _argProgressCallback;
};

void Dbecore::Rst::lockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.lockMutex(srefObject, srefMember, "rref=" + to_string(rref));
};

void Dbecore::Rst::signalProgress(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.signal(srefObject, srefMember, "rref=" + to_string(rref));
};

bool Dbecore::Rst::timedwaitProgress(
			const unsigned int dt
			, const string& srefObject
			, const string& srefMember
		) {
	return cProgress.timedwait(dt, srefObject, srefMember, "rref=" + to_string(rref));
};

void Dbecore::Rst::unlockAccess(
			const string& srefObject
			, const string& srefMember
		) {
	cProgress.unlockMutex(srefObject, srefMember, "rref=" + to_string(rref));
};
