#include <iostream>
#include "ace/OS.h"
#include "ApexClient.h"
// #include "DBC_REQ_Parser.h"

using namespace std;

int main(int argc, char **argv)
{
	cout << "[TRACE] Main.cpp:  init ApexClient" << endl;
	
// 	BYTE msg[MAX_DATA_SIZE] =
// 	{ 
// 		0x81, 0x01, 0x00, 0x46,		// header
// 		0x01, 0x02, 0x03, 0xE8,		// TSID
// 		0x02, 0x06, 0x12, 0x34,		// CMMAC
// 		0x56, 0x78, 0x9A, 0xBC,		// CMMAC
// 		0x03, 0x43,					// DBC, len
// 					0xAB, 0xCD,		// TransactionID
// 		0x01, 0x01,					// NumberOfFragments, FragmentSequenceNumber
// 					0x32, 0x20, 	// 50		DSID Encodings
// 		0x01, 0x03, 0x65, 0x43,		// 50.1		Downstream Service Identifier (DSID)
// 		0x21, 
// 			  0x02, 0x01, 0x00,		// 50.2		Downstream Service Identifier Action
// 		0x04, 0x16,					// 50.4		Multicast Encodings
// 					0x02, 0x06,		// 50.4.2	Multicast CM Interface Mask bitmap 4FFF00000001
// 		0x4F, 0xFF, 0x00, 0x00,
// 		0x00, 0x01, 
// 					0x03, 0x0C,		// 50.4.3	Multicast Group MAC address Encodings
// 		0x12, 0x34, 0x56, 0x78,
// 		0x9A, 0xBC, 0xFE, 0xDC,
// 		0xBA, 0x98, 0x76, 0x54,
// 		0x31, 0x1B,					// 49		Receive Channel Configuration (RCC)
// 					0x01, 0x05,		// 49.1		Receive Channel Profile ID	 0x0010000003 
// 		0x00, 0x10, 0x00, 0x00,
// 		0x03, 
// 			  0x05, 0x12,			// 49.5		Received Channel assigned by CMTS
// 						  0x01,		// 49.5.1	Receive Channel (RC) Index
// 		0x07, 0x01, 0x02, 0x03,
// 		0x04, 0x05, 0x06, 0x07,
// 		0x04, 0x04, 0x00, 0x0D,		// 49.5.4	Receive Channel Center Frequency
// 		0x90, 0x38,
// 					0x05, 0x01,		// 49.5.5	RC Primary DS Channel Indicator
// 		0x00
// 	};
// 
// 	DbcReqParser parser(msg);
// 	parser.Parse();

	UINT port = 9960;
	if (argc >= 2)
		port = atoi(argv[1]);

	APEX_CLIENT_INSTANCE->Init(port);

	ACE_OS::sleep(5);

	APEX_CLIENT_INSTANCE->Close();

	while (true)
		ACE_OS::sleep(10);

	return 0;
}
