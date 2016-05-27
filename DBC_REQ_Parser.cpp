#include "DBC_REQ_Parser.h"
#include <stdio.h>
#include <string.h>

#define ASSERT_TYPE(X, Y) \
{ \
	if((X) != (Y)) \
	{ \
		printf("[ERROR] Type error: %02x != %02x\n", (X), (Y)); \
		return false; \
	} \
}

using namespace std;

DbcReqParser::DbcReqParser(BYTE * buf)
	: m_pTail(NULL)
{
	printf("[TRACE] DbcReqParser::DbcReqParser\n");
	memcpy(m_buf, buf, MAX_DATA_SIZE);
	m_pCurrent = m_buf;
}

DbcReqParser::~DbcReqParser()
{

}

void DbcReqParser::Parse()
{
	printf("[TRACE] DbcReqParser::Parse  Parse begin!\n");

	if (!ParseHeader())
		return;

	if (!ParseTSID())
		return;

	if (!ParseCMMAC())
		return;

	if (!ParsePayload())
		return;
	
	printf("[TRACE] DbcReqParser::Parse  Parse Completed!\n");
}

bool DbcReqParser::ParseHeader()
{
	// 0x81
	BYTE msgType = *m_pCurrent++;
	BYTE version = *m_pCurrent++;
	ULONG length = *m_pCurrent++;
	length <<= 8;
	length += *m_pCurrent++;
	
	if (length >= MAX_DATA_SIZE)
	{
		printf("[ERROR] DbcReqParser::ParseHeader DBC-REQ payload length = %ld\n", length);
		return false;
	}

	m_pTail = m_pCurrent + length;

	printf("Header:\n\tMessageType = 0x%02x, Version = 0x%02x, Length = %ld\n", msgType, version, length);
	
	ASSERT_TYPE(msgType, 0x81);

	return true;
}

bool DbcReqParser::ParseTSID()
{
	// 0x01
	printf("TSID:\n");
	TLV tlv(m_pCurrent);
	m_pCurrent += tlv.GetLength() + 2;

	ASSERT_TYPE(tlv.GetType(), 0x01);
	if (m_pCurrent >= m_pTail)
	{
		printf("[ERROR] TSID length = %ld", tlv.GetLength());
		return false;
	}

	tlv.PrintDec();

	return true;
}

bool DbcReqParser::ParseCMMAC()
{
	// 0x02
	printf("CM MAC:\n");
	TLV tlv(m_pCurrent);
	tlv.PrintHex();
	ASSERT_TYPE(tlv.GetType(), 0x02);

	m_pCurrent += tlv.GetLength() + 2;

	return true;
}

bool DbcReqParser::ParsePayload()
{
	// 0x03
	BYTE PayloadType = *m_pCurrent++;
	ULONG length = *m_pCurrent++;

	UINT TransactionID = *m_pCurrent++ * 256;
	TransactionID += *m_pCurrent++;

	UINT NumberofFragments = *m_pCurrent++;
	UINT FragmentSequenceNumber = *m_pCurrent++;

	printf("DBC-REQ payload:\n\tType = 0x%02x, Length = %ld, \n\tTransaction ID = 0x%04x, Number of Fragments = 0x%02x, Fragment Sequence Number = 0x%02x\n"
		, PayloadType, length, TransactionID, NumberofFragments, FragmentSequenceNumber);
	
	ASSERT_TYPE(PayloadType, 0x03);

	if (!ParseDSIDEncoding())
		return false;
	
	if (!ParseReceiveChannelConfiguration())
		return false;

	return true;
}

bool DbcReqParser::ParseDSIDEncoding()
{
	// 50
	BYTE DSIDType = *m_pCurrent++;
	ULONG length = *m_pCurrent++;
	printf("50 - DSID Encodings:\n\tType = 0x%02x, Length = %ld\n", DSIDType, length);
	ASSERT_TYPE(DSIDType, 50);

	// 50.1
	TLV tlv_50_1(m_pCurrent);
	printf("50.1 - DSID:\n");
	ASSERT_TYPE(tlv_50_1.GetType(), 1);
	tlv_50_1.PrintDec();
	m_pCurrent += tlv_50_1.GetLength() + 2;

	// 50.2
	TLV tlv_50_2(m_pCurrent);
	printf("50.2 - DSID Action:\n");
	ASSERT_TYPE(tlv_50_2.GetType(), 2);
	tlv_50_2.PrintDec();
	m_pCurrent += tlv_50_2.GetLength() + 2;

	// 50.4
	BYTE MulticastType = *m_pCurrent++;
	ULONG MulticastLength = *m_pCurrent++;
	printf("50.4 - Multicast Encodings:\n\tType = 0x%02x, Length = %ld\n", MulticastType, MulticastLength);
	ASSERT_TYPE(MulticastType, 4);

	// 50.4.2
	TLV tlv_50_4_2(m_pCurrent);
	printf("50.4.2 - Multicast CM Interface Mask:\n");
	ASSERT_TYPE(tlv_50_4_2.GetType(), 2);
	tlv_50_4_2.PrintHex();
	m_pCurrent += tlv_50_4_2.GetLength() + 2;

	// 50.4.3
	TLV tlv_50_4_3(m_pCurrent);
	printf("50.4.3 - Multicast Group MAC address:\n");
	ASSERT_TYPE(tlv_50_4_3.GetType(), 3);
	ULONG MacLen = tlv_50_4_3.GetLength();
	printf("\tType = 0x%02x, length = %ld\n", tlv_50_4_3.GetType(), MacLen);
	BYTE* value = tlv_50_4_3.GetValue();
	for (ULONG i = 0; i < MacLen; i++)
	{
		if (0 == i % 6)
			printf("\tGMAC[%ld] = ", i / 6 + 1);
		printf("%02x ", value[i]);
	}
	printf("\n");
	m_pCurrent += MacLen + 2;

	return true;
}

bool DbcReqParser::ParseReceiveChannelConfiguration()
{
	// 49
	BYTE RCCType = *m_pCurrent++;
	ULONG length = *m_pCurrent++;
	printf("49 - Receive Channel Configuration (RCC):\n\tType = 0x%02x, Length = %ld\n", RCCType, length);
	ASSERT_TYPE(RCCType, 49);

	// 49.1
	TLV tlv_49_1(m_pCurrent);
	printf("49.1 - Receive Channel Profile ID:\n");
	ASSERT_TYPE(tlv_49_1.GetType(), 1);
	tlv_49_1.PrintHex();
	m_pCurrent += (tlv_49_1.GetLength() + 2);

	// 49.5
	BYTE RCType = *m_pCurrent++;
	ULONG RCLength = *m_pCurrent++;
	printf("49.5 - Received Channel assigned by CMTS:\n\tType = 0x%02x, Length = %ld\n", RCType, RCLength);
	ASSERT_TYPE(RCType, 5);

	// 49.5.1
	TLV tlv_49_5_1(m_pCurrent);
	printf("49.5.1 - Receive Channel (RC) Index:\n");
	ASSERT_TYPE(tlv_49_5_1.GetType(), 1);
	tlv_49_5_1.PrintHex();
	m_pCurrent += (tlv_49_5_1.GetLength() + 2);

	// 49.5.4
	TLV tlv_49_5_4(m_pCurrent);
	printf("49.5.4 - Receive Channel Center Frequency\n");
	ASSERT_TYPE(tlv_49_5_4.GetType(), 4);
	tlv_49_5_4.PrintDec();
	m_pCurrent += (tlv_49_5_4.GetLength() + 2);

	// 49.5.5
	TLV tlv_49_5_5(m_pCurrent);
	printf("49.5.5 - RC Primary DS Channel Indicator:\n");
	ASSERT_TYPE(tlv_49_5_5.GetType(), 5);
	tlv_49_5_5.PrintDec();
	m_pCurrent += (tlv_49_5_4.GetLength() + 2);

	return true;
}

TLV::TLV(const BYTE * buf)
	: type_(0)
	, length_(0)
{
	if (NULL == buf)
	{
		printf("[ERROR] TLV::TLV  buf is NULL\n");
		return;
	}
	type_ = *buf++;
	length_ = *buf++;
	memcpy(value_, buf, length_);
}

TLV::~TLV()
{

}

void TLV::PrintDec()
{
	ULONG value = 0;
	for (ULONG i = 0; i < length_; i++)
	{
		value <<= 8;
		value += value_[i];
	}
	printf("\tType = 0x%02x, length = %ld, value = %ld\n", type_, length_, value);
}

void TLV::PrintHex()
{
	printf("\tType = 0x%02x, length = %ld, value = 0x", type_, length_);
	for (ULONG i = 0; i < length_; i++)
		printf("%02x ", value_[i]);
	printf("\n");
}

ULONG TLV::GetLength()
{
	return length_;
}

BYTE TLV::GetType()
{
	return type_;
}

BYTE* TLV::GetValue()
{
	return value_;
}
