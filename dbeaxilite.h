#ifndef DBEAXILITE_H
#define DBEAXILITE_H

#include <linux/types.h>

#define DBEAXILITE_IOC_RUNCMD 100
#define DBEAXILITE_IOC_RUNCMDS 101

#define DBEAXILITE_IOC_RUNBUFXF 200

struct dbeaxilite_cmd {
	__u8 tx1[9];
	__u8 tx3[9];
	__u8 rx4[6];

	size_t lenParbufInv;
	__u8* parbufInv;

	size_t lenParbufRet;
	__u8* parbufRet;
};

struct dbeaxilite_cmds {
	size_t NCmd;
	struct dbeaxilite_cmd* cmds;
};

struct dbeaxilite_bufxf {
	__u8 tixVBuffer;

	__u8 writeNotRead;

	size_t reqlen;
	__u8* data;
	size_t* ptr;
};

#endif
