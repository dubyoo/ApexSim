#ifndef __DBC_REQ_PARSER_H__
#define __DBC_REQ_PARSER_H__

#include "BBNTypes.h"

#define MAX_DATA_SIZE 1024

class TLV
{
public:
	TLV(const BYTE * buf);
	~TLV();

	void PrintDec();
	void PrintHex();

	ULONG GetLength();
	BYTE  GetType();
	BYTE* GetValue();

private:
	BYTE  type_;
	ULONG length_;
	BYTE* value_;
};


class DbcReqParser
{
public:
	DbcReqParser(BYTE * buf);
	~DbcReqParser();

	void Parse();

private:
	bool ParseHeader();
	bool ParseTSID();
	bool ParseCMMAC();
	bool ParsePayload();
	bool ParseDSIDEncoding();
	bool ParseReceiveChannelConfiguration();

private:
	BYTE m_buf[MAX_DATA_SIZE];
	BYTE * m_pCurrent;
	BYTE * m_pTail;
};

#endif
