/**
	* \file Cmd.h
	* command functionality (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 3 Feb 2016
	*/

#ifndef DBECORE_CMD_H
#define DBECORE_CMD_H

#include <dbecore/Crc.h>
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

	public:
		Cmd(const uint8_t tixVCommand, const uint32_t ixVRettype, uint32_t (*invparGetIxBySref)(const std::string& sref) = NULL, std::string (*invparGetSrefByIx)(const uint32_t ix) = NULL, uint32_t (*retparGetIxBySref)(const std::string& sref) = NULL, std::string (*retparGetSrefByIx)(const uint32_t ix) = NULL);
		Cmd(const uint8_t tixVController, const uint8_t tixVCommand, const uint32_t ixVRettype, uint32_t (*invparGetIxBySref)(const std::string& sref) = NULL, std::string (*invparGetSrefByIx)(const uint32_t ix) = NULL, uint32_t (*retparGetIxBySref)(const std::string& sref) = NULL, std::string (*retparGetSrefByIx)(const uint32_t ix) = NULL);
		~Cmd();

	public:
		uint8_t tixVController;
		uint8_t tixVCommand;

		uint32_t ixVRettype;

		uint32_t (*invparGetIxBySref)(const std::string& sref);
		std::string (*invparGetSrefByIx)(const uint32_t ix);

		uint32_t (*retparGetIxBySref)(const std::string& sref);
		std::string (*retparGetSrefByIx)(const uint32_t ix);

		std::map<uint32_t, Par> invpars;
		std::map<uint32_t, Par> retpars;

		size_t lenParbufInv;
		uint8_t* parbufInv;

		size_t lenParbufRet;
		uint8_t* parbufRet;

		uint8_t tx1[9];
		uint8_t tx3[9];
		uint8_t rx4[6];

		bool fixeddone;

	public:
		void addInvpar(const uint32_t ix, const uint32_t ixVType, uint8_t (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const uint8_t tix) = NULL, void (*fillFeed)(Sbecore::Feed& feed) = NULL, size_t len = 0);
		void addRetpar(const uint32_t ix, const uint32_t ixVType, uint8_t (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const uint8_t tix) = NULL, void (*fillFeed)(Sbecore::Feed& feed) = NULL, size_t len = 0);

		void composeFixed();

		std::string parsToTemplate(const bool retNotInv);
		std::string getParsText(const bool retNotInv, const bool truncate = false, bool* truncated = NULL); // could replace by getParsText(retNotInv)
		std::string getParsHex(const bool retNotInv, const bool truncate = false, bool* truncated = NULL); // this by getParsHex(retNotInv)

		void resetParbufInv();

		void hexToParbufInv(const std::string& s);
		void parlistToParbufInv(const std::string& s);

		void resetParbufRet();
	};
};
#endif
