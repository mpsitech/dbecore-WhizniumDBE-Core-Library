/**
	* \file Err.h
	* error functionality (declarations)
	* \copyright (C) 2017-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 21 Apr 2017
	*/

#ifndef DBECORE_ERR_H
#define DBECORE_ERR_H

#include <dbecore/Par.h>

namespace Dbecore {
	/**
		* Err
		*/
	class Err {

	public:
		Err(const uint8_t tixDbeVAction = VecDbeVAction::ERR, const uint8_t tixVError = 0x00);
		virtual ~Err();

	public:
		uint8_t tixDbeVAction;
		uint8_t tixVError;

		std::map<std::string, Par> pars;
		std::vector<std::string> seqPars;

	public:
		void addPar(const std::string& sref, const uint32_t ixVType, uint8_t (*getTixBySref)(const std::string& sref) = NULL, std::string (*getSrefByTix)(const uint8_t tix) = NULL, void (*fillFeed)(Sbecore::Feed& feed) = NULL, size_t buflen = 0);

		void bufToPars(const unsigned char* buf, const size_t buflen);

		void parsToBuf(unsigned char** buf, size_t& buflen);

		size_t getBuflen();
		std::string getParText(const bool truncate = false, bool* truncated = NULL);
		std::string getParHex(const bool truncate = false, bool* truncated = NULL);

		std::string getMessage(const std::string& srefCtr, const std::string& srefCmd, const uint32_t cref, const std::string& srefErr, const std::string& titErr, const bool cmdNotErronly, const bool titleNotSref);

	public:
		static Err getNewRteerr();
		static Err getNewCreferr();
		static Err getNewFwderr();
		static Err getNewCmderr();
		static Err getNewToerr();
		static Err getNewRsterr();
	};
};
#endif
