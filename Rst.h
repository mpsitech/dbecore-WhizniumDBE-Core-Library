/**
  * \file Rst.h
  * reset functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 21 Apr 2017
  * \date modified: 29 Apr 2017
  */

#ifndef DBECORE_RST_H
#define DBECORE_RST_H

#include <dbecore/Cmd.h>

/**
  * rstref_t
  */
class rstref_t {

public:
	rstref_t(const uint ixVState = 0, const ubigint rref = 0);

public:
	uint ixVState;
	ubigint rref;

public:
	bool operator<(const rstref_t& comp) const;
};

/**
  * rstref2_t
  */
class rstref2_t {

public:
	rstref2_t(const uint ixVTarget = 0, const ubigint uref = 0, const ubigint rref = 0);

public:
	uint ixVTarget;
	ubigint uref;
	ubigint rref;

public:
	bool operator<(const rstref2_t& comp) const;
};

/**
	* Rst
	*/
class Rst {

public:
	/**
		* VecVState
		*/
	class VecVState {

	public:
		static const uint VOID = 0;
		static const uint WAITPREP = 1;
		static const uint WAITINV = 2;
		static const uint WAITRST = 3;
		static const uint DONE = 4;

		static uint getIx(const string& sref);
		static string getSref(const uint ix);
		static string getTitle(const uint ix);
	};

public:
	Rst(const uint ixVTarget = 0, const ubigint uref = 0);
	~Rst();

public:
	uint ixVTarget;
	ubigint uref;

	uint ixVState;

	ubigint rref;

	bool root;
	Cmd* cmd;

	set<uint> subIcsVTarget;

	Cond cProgress;

	bool (*progressCallback)(Rst* rst, void* arg);
	void* argProgressCallback;

public:
	void setProgressCallback(bool (*_progressCallback)(Rst* rst, void* arg), void* _argProgressCallback);

	void lockAccess(const string& srefObject, const string& srefMember);
	void signalProgress(const string& srefObject, const string& srefMember);
	bool timedwaitProgress(const unsigned int dt, const string& srefObject, const string& srefMember);
	void unlockAccess(const string& srefObject, const string& srefMember);
};

#endif
