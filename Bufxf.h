/**
  * \file Bufxf.h
  * buffer transfer functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 23 Nov 2016
  * \date modified: 29 Apr 2017
  */

#ifndef DBECORE_BUFXFER_H
#define DBECORE_BUFXFER_H

#include <dbecore/Cmd.h>

/**
  * bufxfref_t
  */
class bufxfref_t {

public:
	bufxfref_t(const uint ixVState = 0, const utinyint rootTixWBuffer = 0, const ubigint bref = 0);

public:
	uint ixVState;
	utinyint rootTixWBuffer;
	ubigint bref;

public:
	bool operator<(const bufxfref_t& comp) const;
};

/**
  * bufxfref2_t
  */
class bufxfref2_t {

public:
	bufxfref2_t(const uint ixVTarget = 0, const ubigint uref = 0, const ubigint bref = 0);

public:
	uint ixVTarget;
	ubigint uref;
	ubigint bref;

public:
	bool operator<(const bufxfref2_t& comp) const;
};

/**
	* Bufxf
	*/
class Bufxf {

public:
	/**
		* VecVState
		*/
	class VecVState {

	public:
		static const uint VOID = 0;
		static const uint WAITPREP = 1;
		static const uint WAITINV = 2;
		static const uint WAITXFER = 3;
		static const uint WAITRET = 4;
		static const uint DONE = 5;

		static uint getIx(const string& sref);
		static string getSref(const uint ix);
		static string getTitle(const uint ix);
	};

public:
	Bufxf(const utinyint tixWBuffer, const bool writeNotRead, const size_t reqlen);
	~Bufxf();

public:
	utinyint tixWBuffer;

	bool writeNotRead;

	size_t reqlen;

	uint ixVTarget;
	ubigint uref;

	uint ixVState;

	ubigint bref;

	set<cmdix_t> icsReqcmd;

	utinyint rootTixWBuffer;
	vector<Cmd*> cmds;

	unsigned char* data;
	size_t ptr;

	bool success; // ev. replace by Err

	pthread_mutex_t mAccess;

	bool (*progressCallback)(Bufxf* bufxf, void* arg);
	void* argProgressCallback;

	bool (*errorCallback)(Bufxf* bufxf, void* arg);
	void* argErrorCallback;

	bool (*doneCallback)(Bufxf* bufxf, void* arg);
	void* argDoneCallback;

public:
	void appendReadData(const unsigned char* _data, const size_t _datalen);
	unsigned char* getReadData();
	size_t getReadDatalen();

	void setWriteData(const unsigned char* _data, const size_t _datalen);

	void setProgressCallback(bool (*_progressCallback)(Bufxf* bufxf, void* arg), void* _argProgressCallback);
	void setErrorCallback(bool (*_errorCallback)(Bufxf* bufxf, void* arg), void* _argErrorCallback);
	void setDoneCallback(bool (*_doneCallback)(Bufxf* bufxf, void* arg), void* _argDoneCallback);

	int lockAccess(const string& srefObject, const string& srefMember);
	int unlockAccess(const string& srefObject, const string& srefMember);
};

#endif
