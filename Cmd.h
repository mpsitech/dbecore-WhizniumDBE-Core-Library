/**
  * \file Cmd.h
  * command functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 3 Feb 2016
  * \date modified: 29 Apr 2017
  */

#ifndef DBECORE_CMD_H
#define DBECORE_CMD_H

#include <dbecore/Err.h>
#include <dbecore/Par.h>

/**
  * cmdix_t
  */
class cmdix_t {

public:
	cmdix_t(const uint ixVTarget = 0, const utinyint tixVController = 0, const utinyint tixVCommand = 0);

public:
	uint ixVTarget;
	utinyint tixVController;
	utinyint tixVCommand;

public:
	bool operator<(const cmdix_t& comp) const;
};

/**
  * cmdref_t
  */
class cmdref_t {

public:
	cmdref_t(const uint ixVState = 0, const uint cref = 0);

public:
	uint ixVState;
	uint cref;

public:
	bool operator<(const cmdref_t& comp) const;
};

/**
  * cmdref2_t
  */
class cmdref2_t {

public:
	cmdref2_t(const uint ixVTarget = 0, const ubigint uref = 0, const uint cref = 0);

public:
	uint ixVTarget;
	ubigint uref;
	uint cref;

public:
	bool operator<(const cmdref2_t& comp) const;
};

/**
	* Cmd
	*/
class Cmd {

public:
	/**
		* VecVRettype
		*/
	class VecVRettype {

	public:
		static const uint VOID = 1;
		static const uint IMM = 2;
		static const uint DFR = 3;
		static const uint MULT = 4;

		static uint getIx(const string& sref);
		static string getSref(const uint ix);
		static string getTitle(const uint ix);
	};

	/**
		* VecVState
		*/
	class VecVState {

	public:
		static const uint VOID = 0;
		static const uint WAITINV = 1;
		static const uint WAITREV = 2;
		static const uint WAITRET = 3;
		static const uint WAITNEWRET = 4;
		static const uint DONE = 5;

		static uint getIx(const string& sref);
		static string getSref(const uint ix);
		static string getTitle(const uint ix);
	};

public:
	Cmd(const utinyint tixVCommand, const uint ixVRettype);
	Cmd(const utinyint tixVController, const utinyint tixVCommand, const uint ixVRettype);
	virtual ~Cmd();

public:
	utinyint tixVController;
	utinyint tixVCommand;

	uint ixVRettype;

	uint ixVTarget;
	ubigint uref;

	uint ixVState;

	uint cref;

	map<string,Par> parsInv;
	vector<string> seqParsInv;

	Err err;

	unsigned int Nret;

	map<string,Par> parsRet;
	vector<string> seqParsRet;

	pthread_mutex_t mAccess;

	bool (*progressCallback)(Cmd* cmd, void* arg);
	void* argProgressCallback;

	void (*returnCallback)(Cmd* cmd, void* arg);
	void* argReturnCallback;

	// returnSpeccallback is specific for each subclass

	bool (*errorCallback)(Cmd* cmd, void* arg);
	void* argErrorCallback;

	bool (*doneCallback)(Cmd* cmd, void* arg);
	void* argDoneCallback;

public:
	void addParInv(const string& sref, const uint ixVType, utinyint (*getTixBySref)(const string& sref) = NULL, string (*getSrefByTix)(const utinyint tix) = NULL, void (*fillFeed)(Feed& feed) = NULL, size_t buflen = 0);
	void addParRet(const string& sref, const uint ixVType, utinyint (*getTixBySref)(const string& sref) = NULL, string (*getSrefByTix)(const utinyint tix) = NULL, void (*fillFeed)(Feed& feed) = NULL, size_t buflen = 0);

	void setProgressCallback(bool (*_progressCallback)(Cmd* cmd, void* arg), void* _argProgressCallback);
	void setReturnCallback(void (*_returnCallback)(Cmd* cmd, void* arg), void* _argReturnCallback);
	void setErrorCallback(bool (*_errorCallback)(Cmd* cmd, void* arg), void* _argErrorCallback);
	void setDoneCallback(bool (*_doneCallback)(Cmd* cmd, void* arg), void* _argDoneCallback);

	virtual void returnToCallback();

	int lockAccess(const string& srefObject, const string& srefMember);
	int unlockAccess(const string& srefObject, const string& srefMember);

	void hexToParsInv(const string& s);
	void parlistToParsInv(const string& s);

	void parsInvToBuf(unsigned char** buf, size_t& buflen);
	string parsInvToTemplate();

	size_t getInvBuflen();
	string getInvText(const bool truncate = false, bool* truncated = NULL);
	string getInvHex(const bool truncate = false, bool* truncated = NULL);

	void resetParsRet();

	void bufToParsRet(const unsigned char* buf, const size_t buflen);

	void parsRetToBuf(unsigned char** buf, size_t& buflen);

	size_t getRetBuflen();
	string getRetText(const bool truncate = false, bool* truncated = NULL);
	string getRetHex(const bool truncate = false, bool* truncated = NULL);
};

#endif
