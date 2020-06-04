/**
  * \file Cmd.h
  * command functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 3 Feb 2016
  * \date modified: 18 May 2020
  */

#ifndef DBECORE_CMD_H
#define DBECORE_CMD_H

#include <dbecore/Err.h>

namespace Dbecore {
	/**
		* cmdix_t
		*/
	class cmdix_t {

	public:
		cmdix_t(const Sbecore::uint ixVTarget = 0, const Sbecore::utinyint tixVController = 0, const Sbecore::utinyint tixVCommand = 0);

	public:
		Sbecore::uint ixVTarget;
		Sbecore::utinyint tixVController;
		Sbecore::utinyint tixVCommand;

	public:
		bool operator<(const cmdix_t& comp) const;
	};

	/**
		* cmdref_t
		*/
	class cmdref_t {

	public:
		cmdref_t(const Sbecore::uint ixVState = 0, const Sbecore::uint cref = 0);

	public:
		Sbecore::uint ixVState;
		Sbecore::uint cref;

	public:
		bool operator<(const cmdref_t& comp) const;
	};

	/**
		* cmdref2_t
		*/
	class cmdref2_t {

	public:
		cmdref2_t(const Sbecore::uint ixVTarget = 0, const Sbecore::ubigint uref = 0, const Sbecore::uint cref = 0);

	public:
		Sbecore::uint ixVTarget;
		Sbecore::ubigint uref;
		Sbecore::uint cref;

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
			static const Sbecore::uint VOID = 1;
			static const Sbecore::uint STATSNG = 2;
			static const Sbecore::uint IMMSNG = 3;
			static const Sbecore::uint DFRSNG = 4;
			static const Sbecore::uint MULT = 5;

			static Sbecore::uint getIx(const std::string& sref);
			static std::string getSref(const Sbecore::uint ix);
			static std::string getTitle(const Sbecore::uint ix);
		};

		/**
			* VecVState
			*/
		class VecVState {

		public:
			static const Sbecore::uint VOID = 0;
			static const Sbecore::uint WAITINV = 1;
			static const Sbecore::uint WAITREV = 2;
			static const Sbecore::uint WAITRET = 3;
			static const Sbecore::uint WAITNEWRET = 4;
			static const Sbecore::uint DONE = 5;

			static Sbecore::uint getIx(const std::string& sref);
			static std::string getSref(const Sbecore::uint ix);
			static std::string getTitle(const Sbecore::uint ix);
		};

	public:
		Cmd(const Sbecore::utinyint tixVCommand, const Sbecore::uint ixVRettype);
		Cmd(const Sbecore::utinyint tixVController, const Sbecore::utinyint tixVCommand, const Sbecore::uint ixVRettype);
		virtual ~Cmd();

	public:
		Sbecore::utinyint tixVController;
		Sbecore::utinyint tixVCommand;

		Sbecore::uint ixVRettype;

		Sbecore::uint ixVTarget;
		Sbecore::ubigint uref;

		Sbecore::uint ixVState;

		Sbecore::uint cref;

		std::map<std::string,Par> parsInv;
		std::vector<std::string> seqParsInv;

		Err err;

		unsigned int Nret;

		std::map<std::string,Par> parsRet;
		std::vector<std::string> seqParsRet;

		Sbecore::Cond cProgress;

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
		void addParInv(const std::string& sref, const Sbecore::uint ixVType, Sbecore::utinyint (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const Sbecore::utinyint tix) = NULL, void (*fillFeed)(Sbecore::Xmlio::Feed& feed) = NULL, size_t buflen = 0);
		void addParRet(const std::string& sref, const Sbecore::uint ixVType, Sbecore::utinyint (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const Sbecore::utinyint tix) = NULL, void (*fillFeed)(Sbecore::Xmlio::Feed& feed) = NULL, size_t buflen = 0);

		void setProgressCallback(bool (*_progressCallback)(Cmd* cmd, void* arg), void* _argProgressCallback);
		void setReturnCallback(void (*_returnCallback)(Cmd* cmd, void* arg), void* _argReturnCallback);
		void setErrorCallback(bool (*_errorCallback)(Cmd* cmd, void* arg), void* _argErrorCallback);
		void setDoneCallback(bool (*_doneCallback)(Cmd* cmd, void* arg), void* _argDoneCallback);

		virtual void returnToCallback();

		void lockAccess(const std::string& srefObject, const std::string& srefMember);
		void signalProgress(const std::string& srefObject, const std::string& srefMember);
		void waitProgress(const std::string& srefObject, const std::string& srefMember);
		bool timedwaitProgress(const unsigned int dt, const std::string& srefObject, const std::string& srefMember);
		void unlockAccess(const std::string& srefObject, const std::string& srefMember);

		std::string parsToTemplate(const bool retNotInv);

		void hexToParsInv(const std::string& s);
		void parlistToParsInv(const std::string& s);

		void parsInvToBuf(unsigned char** buf, size_t& buflen);

		size_t getInvBuflen();
		std::string getInvText(const bool truncate = false, bool* truncated = NULL);
		std::string getInvHex(const bool truncate = false, bool* truncated = NULL);

		void resetParsRet();

		void bufToParsRet(const unsigned char* buf, const size_t buflen);

		void parsRetToBuf(unsigned char** buf, size_t& buflen);

		size_t getRetBuflen();
		std::string getRetText(const bool truncate = false, bool* truncated = NULL);
		std::string getRetHex(const bool truncate = false, bool* truncated = NULL);
	};
};
#endif
