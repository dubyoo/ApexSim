/********************************************************************
*			This is part of the ARRIS project.
*	Copyright ARRIS Enterprises, Inc. 1999-2016. All rights reserved.
*
*
*	Description : 	Main.cpp
*			.........................................
*
*	Established And Developed By Dylan 6/01/2016
*
********************************************************************/

#include <iostream>
#include "ace/OS.h"
#include "ApexClient.h"

using namespace std;

int main(int argc, char **argv)
{
	cout << "[TRACE] Main.cpp:  init ApexClient" << endl;

	UINT port = 9960;
	if (argc >= 2)
		port = atoi(argv[1]);

	APEX_CLIENT_INSTANCE->Init(port);

	while (true)
		ACE_OS::sleep(10);

	return 0;
}
