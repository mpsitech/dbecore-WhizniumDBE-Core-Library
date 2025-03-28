/**
	* \file Track.cpp
	* Fsmtrack and Gptrack readout, analysis and output to .vcd (implementation)
	* \copyright (C) 2024 MPSI Technologies GmbH
	* \author Alexander Wirthmueller
	* \date created: 30 Jun 2024
	*/

#include "Track.h"

using namespace std;
using namespace Dbecore;
using namespace Sbecore;

/******************************************************************************
 class Fsmtrack
 ******************************************************************************/

Dbecore::Fsmtrack::Fsmtrack(
			const string& srefModule
			, const string& srefCapture
			, string (*getSrefFst)(const uint8_t tix)
			, const size_t sizeSeqbuf
			, const double fTrkclk
		) :
			srefModule(srefModule)
			, srefCapture(srefCapture)
			, getSrefFst(getSrefFst)
			, sizeSeqbuf(sizeSeqbuf)
			, fTrkclk(fTrkclk)
		{

	cvr.resize(256, false);
};

string Dbecore::Fsmtrack::getSrefFst_showUnknown(
			const uint8_t tix
			, const bool tixFallback
		) {
	string sref;

	sref = getSrefFst(tix);

	if (sref == "") {
		if (tixFallback) sref = "? (0x" + Dbe::binToHex(tix) + ")";
		else sref = "?";
	};

	return("0x" + Dbe::binToHex(tix) + " (" + sref + ")");
};

void Dbecore::Fsmtrack::analyzeCoverage(
			unsigned char* coverage
		) {
	cvr.clear();
	cvr.resize(256, false);

	if (!coverage) return;

	for (unsigned int j = 0; j < cvr.size(); j++) cvr[j] = (coverage[j/8] & (0x01 << (j%8)));
};

void Dbecore::Fsmtrack::analyzeCntbuf(
			unsigned char* cntbuf
		) {
	cnts.clear();
	total = 0;

	if (!cntbuf) return;

	uint32_t cnt;

	for (unsigned int i = 0; i < 256; i++) {//if (cvr[i]) {
		cnt = 0;

		for (unsigned int j = 0; j < 4; j++) cnt = 256*cnt + cntbuf[4*i + (4-j-1)];

		if (cnt != 0) cnts[cnt] = i;

		total += cnt;
	};
};

void Dbecore::Fsmtrack::analyzeFstoccbuf(
			unsigned char* fstoccbuf
		) {
	fstoccs.clear();

	if (!fstoccbuf) return;

	uint32_t fstocc;

	for (unsigned int i = 0; i < 256; i++) if (cvr[i]) {
		fstocc = (fstoccbuf[4*i] << 24) + (fstoccbuf[4*i+1] << 16) + (fstoccbuf[4*i+2] << 8) + fstoccbuf[4*i+3];
		fstoccs[((double) fstocc) / fTrkclk] = i;
	};
};

void Dbecore::Fsmtrack::analyzeSeqbuf(
			unsigned char* seqbuf
			, const unsigned int TMax
			, const size_t ptr0
		) {
	ts.clear();
	vals.clear();

	if (!seqbuf) return;

	unsigned int ish;

	unsigned int t = 0;
	unsigned char quad[4];

	uint8_t state_last = 0xFF;

	for (unsigned int i = 0; i < sizeSeqbuf/4; i++) {
		ish = (i + ptr0) % (sizeSeqbuf/4); 
		for (unsigned int j = 0; j < 4; j++) quad[j] = seqbuf[4*ish + j];

		if (quad[3] == 0xFF) {
			if (quad[2] != state_last) {
				ts.push_back(t);
				vals.push_back(quad[2]);

				state_last = quad[2];
			};

			t += 4 * ((quad[1] << 8) + quad[0]);
			if (t > TMax) break;

		} else {
			for (unsigned int j = 0; j < 4; j++) {
				if (quad[j] != state_last) {
					ts.push_back(t);
					vals.push_back(quad[j]);

					state_last = quad[j];
				};

				t++;
				if (t > TMax) break;
			};

			if (t > TMax) break;
		};
	};

	if  ((t < TMax) && (ptr0 != 0)) for (unsigned int i = 0; i < ts.size(); i++) ts[i] += (TMax - t); // currently use (ptr0 == 0) as indicator for !trackNotFixed
};

string Dbecore::Fsmtrack::statsToString() {
	double p;

	ostringstream str;
	string s;

	bool first;

	str << "states covered:";
	first = true;
	for (unsigned int j = 0; j < cvr.size(); j++) if (cvr[j]) {
		if (first) first = false;
		else str << ",";

		str << " " << getSrefFst_showUnknown(j);
	};
	str << endl;

	str << "states missed:";
	first = true;
	for (unsigned int j = 0; j < cvr.size(); j++) if (!cvr[j]) {
		s = getSrefFst(j);

		if (s != "") {
			if (first) first = false;
			else str << ",";

			str << " " << getSrefFst_showUnknown(j);
		};
	};
	str << endl;
	str << endl;

	str << "first state occurrences in ms:" << endl;
	for (auto it = fstoccs.begin(); it != fstoccs.end(); it++) str << "[" << (1e3 * (it->first)) << "] " << getSrefFst_showUnknown(it->second) << endl;
	str << endl;

	str << "state occurrence count and share:" << endl;
	for (auto it = cnts.begin(); it != cnts.end(); it++) {
		p = ((double) it->second) / ((double) total);

		str << getSrefFst_showUnknown(it->second) << ": " << (100.0 * p) << "\% (" << ((double) (it->second)) << "/" << ((double) total) << ")" << endl;
	};
	str << endl;

	return str.str();
};

void Dbecore::Fsmtrack::getVcd(
			vector<string>& toc
			, vector<unsigned int>& _ts
			, vector<unsigned int>& keys
			, vector<string>& _vals
		) {
	toc.clear();
	_ts.clear();
	keys.clear();
	_vals.clear();

	toc.push_back(srefCapture + ".string");

	if (ts.size() > 0) if (ts[0] != 0) {
		_ts.push_back(0);
		keys.push_back(0);
		_vals.push_back("s<undefined>");
	};

	for (unsigned int i = 0; i < ts.size(); i++) {
		_ts.push_back(ts[i]);
		keys.push_back(0);
		_vals.push_back("s" + getSrefFst(vals[i]));
	};
};

/******************************************************************************
 class Gptrack
 ******************************************************************************/

Dbecore::Gptrack::Gptrack(
			const string& srefModule
			, string (*getSrefCapture)(const uint8_t tix)
			, const size_t sizeSeqbuf
			, const double fTrkclk
		) :
			srefModule(srefModule)
			, getSrefCapture(getSrefCapture)
			, sizeSeqbuf(sizeSeqbuf)
			, fTrkclk(fTrkclk)
		{
	string root;

	string sref;
	size_t ptr;

	for (unsigned int i = 0; i < 15; i++) {
		sref = getSrefCapture(i);
		if (sref == "") break;

		for (ptr = sref.length(); ptr > 0; ptr--) if ((sref[ptr-1] < '0') || (sref[ptr-1] > '9')) break;
		// ptr points to leftmost numeric character

		if (ptr == sref.length()) {
			root = "";

			froms.push_back(i);
			tos.push_back(i);

		} else {
			if (root == "") {
				root = sref.substr(0, ptr);

				froms.push_back(i);
				tos.push_back(i);

			} else {
				if (sref.substr(0, ptr) == root) tos[tos.size() - 1] = i;
				else {
					root = sref.substr(0, ptr);

					froms.push_back(i);
					tos.push_back(i);
				};
			};
		};
	};
};

void Dbecore::Gptrack::analyzeSeqbuf(
			unsigned char* seqbuf
			, const unsigned int TMax
			, const size_t ptr0
		) {
	ts.clear();
	keys.clear();
	vals.clear();

	if (!seqbuf) return;

	unsigned int ish;

	unsigned int t = 0;
	unsigned char quad[4];

	uint16_t capt_test, capt_last;

	for (unsigned int i = 0; i < sizeSeqbuf/4; i++) {
		ish = (i + ptr0) % (sizeSeqbuf/4); 
		for (unsigned int j = 0; j < 4; j++) quad[j] = seqbuf[4*ish + j];

		if (quad[3] & 0x80) {
			capt_test = (((quad[3] << 8) + quad[2]) & 0x7FFF);
			if (i == 0) capt_last = ~capt_test;

			analyzeSeqbuf_compare(t, capt_test, capt_last);

			capt_last = capt_test;

			t += 2 * ((quad[1] << 8) + quad[0]);
			if (t > TMax) break;

		} else {
			capt_test = (((quad[1] << 8) + quad[0]) & 0x7FFF);
			if (i == 0) capt_last = ~capt_test;

			analyzeSeqbuf_compare(t, capt_test, capt_last);

			capt_last = capt_test;

			t++;
			if (t > TMax) break;

			capt_test = (((quad[3] << 8) + quad[2]) & 0x7FFF);

			analyzeSeqbuf_compare(t, capt_test, capt_last);

			capt_last = capt_test;

			t++;
			if (t > TMax) break;
		};
	};

	if  ((t < TMax) && (ptr0 != 0)) for (unsigned int i = 0; i < ts.size(); i++) ts[i] += (TMax - t); // currently use (ptr0 == 0) as indicator for !trackNotFixed
};

void Dbecore::Gptrack::analyzeSeqbuf_compare(
			const unsigned int t
			, const uint16_t capt_test
			, const uint16_t capt_last
		) {
	uint16_t mask;
	bool found;

	for (unsigned int k = 0; k < froms.size(); k++) {
		mask = 0;
		found = false;

		for (unsigned int l = froms[k]; l <= tos[k]; l++) {
			mask = (mask << 1) + (0x0001 << froms[k]);
			if ((capt_test & mask) != (capt_last & mask)) found = true;
		};

		if (found) {
			ts.push_back(t);
			keys.push_back(k);
			vals.push_back((capt_test & mask) >> froms[k]);
		};
	};
};

void Dbecore::Gptrack::getVcd(
			vector<string>& toc
			, vector<unsigned int>& _ts
			, vector<unsigned int>& _keys
			, vector<string>& _vals
		) {
	uint16_t mask;

	string sref;
	size_t ptr;

	string s;

	toc.clear();
	_ts.clear();
	_keys.clear();
	_vals.clear();

	for (unsigned int i = 0; i < froms.size(); i++) {
		if (froms[i] == tos[i]) toc.push_back(getSrefCapture(froms[i]) + ".wire.1");
		else {
			sref = getSrefCapture(froms[i]);

			for (ptr = sref.length(); ptr > 0; ptr--) if ((sref[ptr-1] < '0') || (sref[ptr-1] > '9')) break;
			// ptr points to leftmost numeric character

			toc.push_back(sref.substr(0, ptr) + "[" + to_string(tos[i] - froms[i]) + ":0].wire." + to_string(tos[i] - froms[i] + 1));
		};
	};

	if (ts.size() > 0) if (ts[0] != 0) {
		for (unsigned int k = 0; k < froms.size(); k++) {
			_ts.push_back(0);
			_keys.push_back(k);

			s = "b" + string(tos[k] - froms[k] + 1, 'U');
			_vals.push_back(s);
		};
	};

	for (unsigned int i = 0; i < ts.size(); i++) {
		_ts.push_back(ts[i]);
		_keys.push_back(keys[i]);

		if (froms[keys[i]] == tos[keys[i]]) s = ((vals[i] == 1) ? "b1" : "b0");
		else {
			mask = 0x0001;

			s = "";

			for (unsigned int l = froms[keys[i]]; l <= tos[keys[i]]; l++) {
				if (vals[i] & mask) s = "1" + s;
				else s = "0" + s;

				mask <<= 1;
			};

			s = "b" + s;
		};

		_vals.push_back(s);
	};
};

/******************************************************************************
 class Vcdwr
 ******************************************************************************/

Dbecore::Vcdwr::Vcdwr(
			const string& Prjshort
			, const string& Untshort
			, const double TBase
			, const unsigned int denomBase
		) :
			Prjshort(Prjshort)
			, Untshort(Untshort)
			, TBase(TBase)
			, denomBase(denomBase)
		{

};

void Dbecore::Vcdwr::append(
			vector<string>& _toc
			, const unsigned int ofs // in base T units
			, const unsigned int num // e.g. num/denom = 3/3 for mclk = 100 MHz, 1/3 for memclk = 300 MHz, base T 3.33 ns
			, vector<unsigned int>& _ts
			, vector<unsigned int>& _keys
			, vector<string>& _vals
		) {
	bool newNotOld;

	vector<unsigned int> ts_old = ts;
	vector<unsigned int> keys_old = keys;
	vector<string> vals_old = vals;

	unsigned int ofsToc = toc.size();
	toc.insert(toc.end(), _toc.begin(), _toc.end());

	ts.clear();
	keys.clear();
	vals.clear();

	unsigned int i = 0;
	unsigned int j = 0;

	while ((i < ts_old.size()) || (j < _ts.size())) {
		if (i == ts_old.size()) newNotOld = true;
		else if (j == _ts.size()) newNotOld = false;
		else newNotOld = ((ofs + num * _ts[j]) <= ts_old[i]);

		if (!newNotOld) {
			ts.push_back(ts_old[i]);
			keys.push_back(keys_old[i]);
			vals.push_back(vals_old[i]);

			i++;

		} else {
			ts.push_back(ofs + num * _ts[j]);
			keys.push_back(_keys[j] + ofsToc);
			vals.push_back(_vals[j]);

			j++;
		};
	};
};

void Dbecore::Vcdwr::clear() {
	toc.clear();
	ts.clear();
	keys.clear();
	vals.clear();
};

void Dbecore::Vcdwr::writeVcd(
			const string& path
			, const unsigned int TMax
			) {
	fstream vcdfile;

	unsigned int TBase_int;

	vector<string> ss;

	time_t now;
	time(&now);

	unsigned int t = 0;
	t--;

	vcdfile.open(path.c_str(), ios::out);

	vcdfile << "$version " << Prjshort << " tracker $end" << endl;
	vcdfile << "$date " << Ftm::stamp(now) << " $end" << endl;

	vcdfile << "$timescale ";
	if (TBase < 1e-8) {
		TBase_int = lround(1e12*TBase);
		if ((TBase_int % 1000) == 0) vcdfile << (TBase_int/1000) << "ns";
		else vcdfile << TBase_int << "ps";

	} else if (TBase < 1e-5) {
		TBase_int = lround(1e9*TBase);
		if ((TBase_int % 1000) == 0) vcdfile << (TBase_int/1000) << "us";
		else vcdfile << TBase_int << "ns";

	} else if (TBase < 1e-2) {
		TBase_int = lround(1e6*TBase);
		if ((TBase_int % 1000) == 0) vcdfile << (TBase_int/1000) << "ms";
		else vcdfile << TBase_int << "us";

	} else if (TBase < 1e1) {
		TBase_int = lround(1e3*TBase);
		if ((TBase_int % 1000) == 0) vcdfile << (TBase_int/1000) << "s";
		else vcdfile << TBase_int << "ms";

	} else vcdfile << lround(TBase) << "s";

	vcdfile << " $end" << endl;
	vcdfile << "$scope module " << Untshort << " $end" << endl;

	for (unsigned int i = 0; i < toc.size(); i++) {
		StrMod::stringToVector(toc[i], ss, '.');

		if (ss.size() <= 2) vcdfile << "\t\t$var string 1";
		else vcdfile << "\t\t$var wire " << ss[2];

		vcdfile << " " << ((char) (0x41+i)) << " " << ss[0] << " $end" << endl;
	};

	vcdfile << "$upscope $end" << endl;
	vcdfile << "$enddefinitions $end" << endl;
	vcdfile << "$dumpvars" << endl;

	for (unsigned int i = 0; i < ts.size(); i++) {
		if (ts[i] != t) {
			t = ts[i];
			vcdfile << "#" << t << endl;
		};

		vcdfile << vals[i] << " " << ((char) (0x41 + keys[i])) << endl;
	};

	if (t < TMax) vcdfile << "#" << TMax << endl;

	vcdfile.close();
};
