/**
  * \file Err.h
  * error functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 21 Apr 2017
  * \date modified: 4 May 2017
  */

#ifndef DBECORE_ERR_H
#define DBECORE_ERR_H

#include <dbecore/Par.h>

/**
  * Err
  */
class Err {

public:
	Err(const utinyint tixDbeVAction = VecDbeVAction::ERR, const utinyint tixVError = 0x00);
	virtual ~Err();

public:
	utinyint tixDbeVAction;
	utinyint tixVError;

	map<string,Par> pars;
	vector<string> seqPars;

public:
	void addPar(const string& sref, const uint ixVType, utinyint (*getTixBySref)(const string& sref) = NULL, string (*getSrefByTix)(const utinyint tix) = NULL, void (*fillFeed)(Feed& feed) = NULL, size_t buflen = 0);

	void bufToPars(const unsigned char* buf, const size_t buflen);

	void parsToBuf(unsigned char** buf, size_t& buflen);

	size_t getBuflen();
	string getParText(const bool truncate = false, bool* truncated = NULL);
	string getParHex(const bool truncate = false, bool* truncated = NULL);

	string getMessage(const string& srefCtr, const string& srefCmd, const uint cref, const string& srefErr, const string& titErr, const bool cmdNotErronly, const bool titleNotSref);

public:
	static Err getNewRteerr();
	static Err getNewCreferr();
	static Err getNewFwderr();
	static Err getNewCmderr();
	static Err getNewToerr();
	static Err getNewRsterr();
};

#endif
