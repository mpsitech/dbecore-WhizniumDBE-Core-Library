/**
	* \file Cmd.h
	* command functionality (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 3 Feb 2016
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
		cmdix_t(const uint32_t ixVTarget = 0, const uint8_t tixVController = 0, const uint8_t tixVCommand = 0);

	public:
		uint32_t ixVTarget;
		uint8_t tixVController;
		uint8_t tixVCommand;

	public:
		bool operator<(const cmdix_t& comp) const;
	};

	/**
		* cmdref_t
		*/
	class cmdref_t {

	public:
		cmdref_t(const uint32_t ixVState = 0, const uint32_t cref = 0);

	public:
		uint32_t ixVState;
		uint32_t cref;

	public:
		bool operator<(const cmdref_t& comp) const;
	};

	/**
		* cmdref2_t
		*/
	class cmdref2_t {

	public:
		cmdref2_t(const uint32_t ixVTarget = 0, const uint64_t uref = 0, const uint32_t cref = 0);

	public:
		uint32_t ixVTarget;
		uint64_t uref;
		uint32_t cref;

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
			static constexpr uint32_t VOID = 1;
			static constexpr uint32_t STATSNG = 2;
			static constexpr uint32_t IMMSNG = 3;
			static constexpr uint32_t DFRSNG = 4;
			static constexpr uint32_t MULT = 5;

			static uint32_t getIx(const std::string& sref);
			static std::string getSref(const uint32_t ix);
			static std::string getTitle(const uint32_t ix);
		};

		/**
			* VecVState
			*/
		class VecVState {

		public:
			static constexpr uint32_t VOID = 0;
			static constexpr uint32_t WAITINV = 1;
			static constexpr uint32_t WAITREV = 2;
			static constexpr uint32_t WAITRET = 3;
			static constexpr uint32_t WAITNEWRET = 4;
			static constexpr uint32_t DONE = 5;

			static uint32_t getIx(const std::string& sref);
			static std::string getSref(const uint32_t ix);
			static std::string getTitle(const uint32_t ix);
		};

	public:
		Cmd(const uint8_t tixVCommand, const uint32_t ixVRettype);
		Cmd(const uint8_t tixVController, const uint8_t tixVCommand, const uint32_t ixVRettype);
		virtual ~Cmd();

	public:
		uint8_t tixVController;
		uint8_t tixVCommand;

		uint32_t ixVRettype;

		uint32_t ixVTarget;
		uint64_t uref;

		uint32_t ixVState;

		uint32_t cref;

		std::map<std::string, Par> parsInv;
		std::vector<std::string> seqParsInv;

		Err err;

		unsigned int Nret;

		std::map<std::string, Par> parsRet;
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
		void addParInv(const std::string& sref, const uint32_t ixVType, uint8_t (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const uint8_t tix) = NULL, void (*fillFeed)(Sbecore::Feed& feed) = NULL, size_t buflen = 0);
		void addParRet(const std::string& sref, const uint32_t ixVType, uint8_t (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const uint8_t tix) = NULL, void (*fillFeed)(Sbecore::Feed& feed) = NULL, size_t buflen = 0);

		void setProgressCallback(bool (*_progressCallback)(Cmd* cmd, void* arg), void* _argProgressCallback);
		void setReturnCallback(void (*_returnCallback)(Cmd* cmd, void* arg), void* _argReturnCallback);
		void setErrorCallback(bool (*_errorCallback)(Cmd* cmd, void* arg), void* _argErrorCallback);
		void setDoneCallback(bool (*_doneCallback)(Cmd* cmd, void* arg), void* _argDoneCallback);

		virtual void returnToCallback();

		void lockAccess(const std::string& srefObject = "", const std::string& srefMember = "");
		void signalProgress(const std::string& srefObject = "", const std::string& srefMember = "");
		void waitProgress(const std::string& srefObject = "", const std::string& srefMember = "");
		bool timedwaitProgress(const unsigned int dt, const std::string& srefObject = "", const std::string& srefMember = "");
		void unlockAccess(const std::string& srefObject = "", const std::string& srefMember = "");

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
