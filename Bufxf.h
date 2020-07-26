/**
  * \file Bufxf.h
  * buffer transfer functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 23 Nov 2016
  * \date modified: 30 Apr 2020
  */

#ifndef DBECORE_BUFXFER_H
#define DBECORE_BUFXFER_H

#include <dbecore/Cmd.h>

namespace Dbecore {
	/**
		* bufxfref_t
		*/
	class bufxfref_t {

	public:
		bufxfref_t(const Sbecore::uint ixVState = 0, const Sbecore::utinyint rootTixWBuffer = 0, const Sbecore::ubigint bref = 0);

	public:
		Sbecore::uint ixVState;
		Sbecore::utinyint rootTixWBuffer;
		Sbecore::ubigint bref;

	public:
		bool operator<(const bufxfref_t& comp) const;
	};

	/**
		* bufxfref2_t
		*/
	class bufxfref2_t {

	public:
		bufxfref2_t(const Sbecore::uint ixVTarget = 0, const Sbecore::ubigint uref = 0, const Sbecore::ubigint bref = 0);

	public:
		Sbecore::uint ixVTarget;
		Sbecore::ubigint uref;
		Sbecore::ubigint bref;

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
			static const Sbecore::uint VOID = 0;
			static const Sbecore::uint WAITPREP = 1;
			static const Sbecore::uint WAITINV = 2;
			static const Sbecore::uint WAITXFER = 3;
			static const Sbecore::uint WAITRET = 4;
			static const Sbecore::uint DONE = 5;

			static Sbecore::uint getIx(const std::string& sref);
			static std::string getSref(const Sbecore::uint ix);
			static std::string getTitle(const Sbecore::uint ix);
		};

	public:
		Bufxf(const Sbecore::utinyint tixWBuffer, const bool writeNotRead, const size_t reqlen, const size_t prelen = 0, const size_t postlen = 0, unsigned char* buf = NULL);
		~Bufxf();

	public:
		Sbecore::utinyint tixWBuffer;

		bool writeNotRead;

		size_t reqlen;
		size_t prelen;
		size_t postlen;

		Sbecore::uint ixVTarget;
		Sbecore::ubigint uref;

		Sbecore::uint ixVState;

		Sbecore::ubigint bref;

		std::set<cmdix_t> icsReqcmd;

		Sbecore::utinyint rootTixWBuffer;
		std::vector<Cmd*> cmds;

		bool dataExtNotInt;
		unsigned char* data;
		size_t ptr;

		bool success; // ev. replace by Err

		Sbecore::Cond cProgress;

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

		void lockAccess(const std::string& srefObject = "", const std::string& srefMember = "");
		void signalProgress(const std::string& srefObject = "", const std::string& srefMember = "");
		bool timedwaitProgress(const unsigned int dt, const std::string& srefObject = "", const std::string& srefMember = "");
		void unlockAccess(const std::string& srefObject = "", const std::string& srefMember = "");
	};
};
#endif
