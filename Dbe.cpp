/**
  * \file Dbe.cpp
  * WhizniumDBE globals (implementation)
  * \author Alexander Wirthmüller
  * \date created: 20 Nov 2016
  * \date modified: 22 Apr 2017
  */

#include "Dbe.h"

/******************************************************************************
 namespace VecDbeVAction
 ******************************************************************************/

utinyint VecDbeVAction::getTix(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s == "inv") return INV;
	else if (s == "rev") return REV;
	else if (s == "ret") return RET;
	else if (s == "newret") return NEWRET;
	else if (s == "err") return ERR;
	else if (s == "rteerr") return RTEERR;
	else if (s == "creferr") return CREFERR;
	else if (s == "fwderr") return FWDERR;
	else if (s == "cmderr") return CMDERR;
	else if (s == "toerr") return TOERR;
	else if (s == "rsterr") return RSTERR;

	return 0;
};

string VecDbeVAction::getSref(
			const utinyint tix
		) {
	if (tix == INV) return("inv");
	else if (tix == REV) return("rev");
	else if (tix == RET) return("ret");
	else if (tix == NEWRET) return("newret");
	else if (tix == ERR) return("err");
	else if (tix == RTEERR) return("rteerr");
	else if (tix == CREFERR) return("creferr");
	else if (tix == FWDERR) return("fwderr");
	else if (tix == CMDERR) return("cmderr");
	else if (tix == TOERR) return("toeer");
	else if (tix == RSTERR) return("rsterr");

	return("");
};

string VecDbeVAction::getTitle(
			const utinyint tix
		) {
	if (tix == INV) return("invoke");
	else if (tix == REV) return("revoke");
	else if (tix == RET) return("return");
	else if (tix == NEWRET) return("new return");
	else if (tix == ERR) return("unit-/controller-specific error");
	else if (tix == RTEERR) return("route target not found error");
	else if (tix == CREFERR) return("conflicting cref on invoke/revoke error");
	else if (tix == FWDERR) return("forwarding controller error");
	else if (tix == CMDERR) return("invalid command error");
	else if (tix == TOERR) return("timeout error");
	else if (tix == RSTERR) return("target reset error");

	return("");
};

/******************************************************************************
 namespace VecDbeVXfer
 ******************************************************************************/

utinyint VecDbeVXfer::getTix(
			const string& sref
		) {
	string s = StrMod::lc(sref);

	if (s == "void") return VOID;
	else if (s == "tkn") return TKN;
	else if (s == "tknste") return TKNSTE;
	else if (s == "avlbx") return AVLBX;
	else if (s == "reqbx") return REQBX;
	else if (s == "arbbx") return ARBBX;
	else if (s == "avllen") return AVLLEN;
	else if (s == "reqlen") return REQLEN;
	else if (s == "arblen") return ARBLEN;
	else if (s == "rd") return RD;
	else if (s == "rdack") return RDACK;
	else if (s == "wr") return WR;
	else if (s == "wrack") return WRACK;

	return 0;
};

string VecDbeVXfer::getSref(
			const utinyint tix
		) {
	if (tix == VOID) return("void");
	else if (tix == TKN) return("tkn");
	else if (tix == TKNSTE) return("tknste");
	else if (tix == AVLBX) return("avlbx");
	else if (tix == REQBX) return("reqbx");
	else if (tix == ARBBX) return("arbbx");
	else if (tix == AVLLEN) return("avllen");
	else if (tix == REQLEN) return("reqlen");
	else if (tix == ARBLEN) return("arblen");
	else if (tix == RD) return("rd");
	else if (tix == RDACK) return("rdack");
	else if (tix == WR) return("wr");
	else if (tix == WRACK) return("wrack");

	return("");
};

/******************************************************************************
 namespace Dbe
 ******************************************************************************/

bool Dbe::bigendian() {
	unsigned short int var = 255;
	char* buf = ((char*) &var);
	
	return(buf[0] == 0);
};

string Dbe::binToHex(
			unsigned char bin
		) {
	string hex;

	unsigned char c;

	c = (bin >> 4) + 0x30;
	if (c > 0x39) c += 0x07;
	hex += (char) c;

	c = (bin & 0x0F) + 0x30;
	if (c > 0x39) c += 0x07;
	hex += (char) c;
	
	return hex;
};

unsigned char Dbe::hexToBin(
			string hex
		) {
	if (hex.size() != 2) return 0;

	unsigned char c, bin;

	c = hex[0];
	if ((c >= '0') && (c <= '9')) c -= 0x30;
	else if ((c >= 'A') && (c <= 'F')) c = c - 0x41 + 0x0A;
	else if ((c >= 'a') && (c <= 'f')) c = c - 0x61 + 0x0A;
	else c = 0;
	bin = (c << 4);

	c = hex[1];
	if ((c >= '0') && (c <= '9')) c -= 0x30;
	else if ((c >= 'A') && (c <= 'F')) c = c - 0x41 + 0x0A;
	else if ((c >= 'a') && (c <= 'f')) c = c - 0x61 + 0x0A;
	else c = 0;
	bin += c;

	return bin;
};

void Dbe::hexToBuf(
			const string& s
			, unsigned char** buf
			, size_t& buflen
		) {
	// examples for s:  "0x01ffbcDA", "FCFCFF078E"

	unsigned int i;

	buflen = s.length()/2;

	i = 0;
	
	if (s.find("0x") == 0) {
		buflen--;
		i++;
	};

	*buf = new unsigned char[buflen];
	for (; i < buflen; i++) (*buf)[i] = hexToBin(s.substr(2*i, 2));
};

string Dbe::bufToHex(
			unsigned char* buf
			, size_t buflen
			, const bool truncate
			, bool* truncated
		) {
	string hex;

	if (buf) {
		if ((buflen > 256) && truncate) {
			if (truncated) *truncated = true;

			for (size_t i = 0; i < 64; i++) hex += binToHex(buf[i]);
			hex += " ... (total: " + to_string(buflen) + " bytes) ... ";
			for (size_t i = (buflen - 64); i < buflen; i++) hex += binToHex(buf[i]);

		} else for (size_t i = 0; i < buflen; i++) hex += binToHex(buf[i]);
	};

	return hex;
};
