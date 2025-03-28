/**
	* \file Track.h
	* Fsmtrack and Gptrack readout, analysis and output to .vcd (declarations)
	* \copyright (C) 2024 MPSI Technologies GmbH
	* \author Alexander Wirthmueller
	* \date created: 30 Jun 2024
	*/

#ifndef DBECORE_TRACK_H
#define DBECORE_TRACK_H

#include <dbecore/Dbe.h>

namespace Dbecore {
	/**
		* Fsmtrack
		*/
	class Fsmtrack {

	public:
		std::string srefModule;
		std::string srefCapture;

		std::string (*getSrefFst)(const uint8_t tix);

		size_t sizeSeqbuf;
		double fTrkclk; // in Hz

		std::vector<bool> cvr; // analyzeCoverage() result

		std::map<uint32_t, uint8_t> cnts; // analyzeCntbuf() result
		uint64_t total;

		std::map<double, uint8_t> fstoccs; // analyzeFstoccbuf() result

		std::vector<unsigned int> ts; // analyzeSeqbuf() result
		std::vector<uint8_t> vals;

	public:
		Fsmtrack(const std::string& srefModule, const std::string& srefCapture, std::string (*getSrefFst)(const uint8_t tix), const size_t sizeSeqbuf, const double fTrkclk);

	public:
		std::string getSrefFst_showUnknown(const uint8_t tix, const bool tixFallback = false);

		void analyzeCoverage(unsigned char* coverage); // 256
		void analyzeCntbuf(unsigned char* cntbuf); // 256 * 4
		void analyzeFstoccbuf(unsigned char* fstoccbuf); // 256 * 4
		void analyzeSeqbuf(unsigned char* seqbuf, const unsigned int TMax, const size_t ptr0 = 0);

		std::string statsToString();
		void getVcd(std::vector<std::string>& toc, std::vector<unsigned int>& _ts, std::vector<unsigned int>& keys, std::vector<std::string>& _vals);
	};

	/**
		* Gptrack
		*/
	class Gptrack {

	public:
		std::string srefModule;

		std::string (*getSrefCapture)(const uint8_t tix);

		size_t sizeSeqbuf;
		double fTrkclk; // in Hz

		std::vector<uint8_t> froms; // capture bits sub-division into single vs. multi-digit values
		std::vector<uint8_t> tos;

		std::vector<unsigned int> ts; // analyzeSeqbuf() result
		std::vector<uint8_t> keys; // froms/tos index
		std::vector<uint16_t> vals;

	public:
		Gptrack(const std::string& srefModule, std::string (*getSrefCapture)(const uint8_t tix), const size_t sizeSeqbuf, const double fTrkclk);

	public:
		void analyzeSeqbuf(unsigned char* seqbuf, const unsigned int TMax, const size_t ptr0 = 0);
		void analyzeSeqbuf_compare(const unsigned int t, const uint16_t capt_test, const uint16_t capt_last);

		void getVcd(std::vector<std::string>& toc, std::vector<unsigned int>& _ts, std::vector<unsigned int>& _keys, std::vector<std::string>& _vals);
	};

	/**
		* Vcdwr
		*/
	class Vcdwr {

	public:
		std::string Prjshort;
		std::string Untshort;

		double TBase;
		unsigned int denomBase;

		std::vector<std::string> toc;
		std::vector<unsigned int> ts;
		std::vector<unsigned int> keys;
		std::vector<std::string> vals;

	public:
		Vcdwr(const std::string& Prjshort, const std::string& Untshort, const double TBase, const unsigned int denomBase);

	public:
		void append(std::vector<std::string>& toc, const unsigned int ofs, const unsigned int num, std::vector<unsigned int>& ts, std::vector<unsigned int>& keys, std::vector<std::string>& vals);
		void clear();

		void writeVcd(const std::string& path, const unsigned int TMax = 0);
	};
};
#endif
