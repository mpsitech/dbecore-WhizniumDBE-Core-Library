/**
  * \file Rst.h
  * reset functionality (declarations)
  * \author Alexander Wirthmüller
  * \date created: 21 Apr 2017
  * \date modified: 30 Apr 2020
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
		rstref_t(const Sbecore::uint ixVState = 0, const Sbecore::ubigint rref = 0);

	public:
		Sbecore::uint ixVState;
		Sbecore::ubigint rref;

	public:
		bool operator<(const rstref_t& comp) const;
	};

	/**
		* rstref2_t
		*/
	class rstref2_t {

	public:
		rstref2_t(const Sbecore::uint ixVTarget = 0, const Sbecore::ubigint uref = 0, const Sbecore::ubigint rref = 0);

	public:
		Sbecore::uint ixVTarget;
		Sbecore::ubigint uref;
		Sbecore::ubigint rref;

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
			static const Sbecore::uint VOID = 0;
			static const Sbecore::uint WAITPREP = 1;
			static const Sbecore::uint WAITINV = 2;
			static const Sbecore::uint WAITRST = 3;
			static const Sbecore::uint DONE = 4;

			static Sbecore::uint getIx(const std::string& sref);
			static std::string getSref(const Sbecore::uint ix);
			static std::string getTitle(const Sbecore::uint ix);
		};

	public:
		Rst(const Sbecore::uint ixVTarget = 0, const Sbecore::ubigint uref = 0);
		~Rst();

	public:
		Sbecore::uint ixVTarget;
		Sbecore::ubigint uref;

		Sbecore::uint ixVState;

		Sbecore::ubigint rref;

		bool root;
		Cmd* cmd;

		std::set<Sbecore::uint> subIcsVTarget;

		Sbecore::Cond cProgress;

		bool (*progressCallback)(Rst* rst, void* arg);
		void* argProgressCallback;

	public:
		void setProgressCallback(bool (*_progressCallback)(Rst* rst, void* arg), void* _argProgressCallback);

		void lockAccess(const std::string& srefObject, const std::string& srefMember);
		void signalProgress(const std::string& srefObject, const std::string& srefMember);
		bool timedwaitProgress(const unsigned int dt, const std::string& srefObject, const std::string& srefMember);
		void unlockAccess(const std::string& srefObject, const std::string& srefMember);
	};
};
#endif
