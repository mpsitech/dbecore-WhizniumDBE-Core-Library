/**
	* \file Rst.h
	* reset functionality (declarations)
	* \copyright (C) 2017-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 21 Apr 2017
	*/

#ifndef DBECORE_RST_H
#define DBECORE_RST_H

#include <dbecore/Cmd.h>

namespace Dbecore {
	/**
		* rstref_t
		*/
	class rstref_t {

	public:
		rstref_t(const uint32_t ixVState = 0, const uint64_t rref = 0);

	public:
		uint32_t ixVState;
		uint64_t rref;

	public:
		bool operator<(const rstref_t& comp) const;
	};

	/**
		* rstref2_t
		*/
	class rstref2_t {

	public:
		rstref2_t(const uint32_t ixVTarget = 0, const uint64_t uref = 0, const uint64_t rref = 0);

	public:
		uint32_t ixVTarget;
		uint64_t uref;
		uint64_t rref;

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
			static constexpr uint32_t VOID = 0;
			static constexpr uint32_t WAITPREP = 1;
			static constexpr uint32_t WAITINV = 2;
			static constexpr uint32_t WAITRST = 3;
			static constexpr uint32_t DONE = 4;

			static uint32_t getIx(const std::string& sref);
			static std::string getSref(const uint32_t ix);
			static std::string getTitle(const uint32_t ix);
		};

	public:
		Rst(const uint32_t ixVTarget = 0, const uint64_t uref = 0);
		~Rst();

	public:
		uint32_t ixVTarget;
		uint64_t uref;

		uint32_t ixVState;

		uint64_t rref;

		bool root;
		Cmd* cmd;

		std::set<uint32_t> subIcsVTarget;

		Sbecore::Cond cProgress;

		bool (*progressCallback)(Rst* rst, void* arg);
		void* argProgressCallback;

	public:
		void setProgressCallback(bool (*_progressCallback)(Rst* rst, void* arg), void* _argProgressCallback);

		void lockAccess(const std::string& srefObject = "", const std::string& srefMember = "");
		void signalProgress(const std::string& srefObject = "", const std::string& srefMember = "");
		bool timedwaitProgress(const unsigned int dt, const std::string& srefObject = "", const std::string& srefMember = "");
		void unlockAccess(const std::string& srefObject = "", const std::string& srefMember = "");
	};
};
#endif
