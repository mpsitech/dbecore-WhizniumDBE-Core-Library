/**
	* \file Bufxf.h
	* buffer transfer functionality (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 23 Nov 2016
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
		bufxfref_t(const uint32_t ixVState = 0, const uint8_t rootTixWBuffer = 0, const uint64_t bref = 0);

	public:
		uint32_t ixVState;
		uint8_t rootTixWBuffer;
		uint64_t bref;

	public:
		bool operator<(const bufxfref_t& comp) const;
	};

	/**
		* bufxfref2_t
		*/
	class bufxfref2_t {

	public:
		bufxfref2_t(const uint32_t ixVTarget = 0, const uint64_t uref = 0, const uint64_t bref = 0);

	public:
		uint32_t ixVTarget;
		uint64_t uref;
		uint64_t bref;

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
			static constexpr uint32_t VOID = 0;
			static constexpr uint32_t WAITPREP = 1;
			static constexpr uint32_t WAITINV = 2;
			static constexpr uint32_t WAITXFER = 3;
			static constexpr uint32_t WAITRET = 4;
			static constexpr uint32_t DONE = 5;

			static uint32_t getIx(const std::string& sref);
			static std::string getSref(const uint32_t ix);
			static std::string getTitle(const uint32_t ix);
		};

	public:
		Bufxf(const uint8_t tixVBuffer, const bool writeNotRead, const size_t reqlen, const size_t prelen = 0, const size_t postlen = 0, unsigned char* buf = NULL);
		~Bufxf();

	public:
		uint8_t tixVBuffer;

		bool writeNotRead;

		size_t reqlen;
		size_t prelen;
		size_t postlen;

		uint32_t ixVTarget;
		uint64_t uref;

		uint32_t ixVState;

		uint64_t bref;

		std::set<cmdix_t> icsReqcmd;

		uint8_t rootTixWBuffer;
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
