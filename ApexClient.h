#ifndef __APEX_CLIENT_H__
#define __APEX_CLIENT_H__

#include "DBC_REQ_UDPSession.h"
#include "BBNTypes.h"
#include "ace/Thread_Manager.h"
#include "ace/Synch.h"

class ApexClient
{
public:
	ApexClient();

	virtual ~ApexClient();
	/*
	Function:
	1. initial udp socket
	2. start a thread to listen and read message from the port of udp socket.
	*/
	HRESULT Init(USHORT ListenPort = 0);

	/*
	Function: to close the udp socket
	*/
	virtual HRESULT Close();

protected:

	static ACE_THR_FUNC_RETURN ThreadProc(void* pParam);
	virtual ACE_THR_FUNC_RETURN RunThreadLoop();

	HRESULT Release();

	
private:
	// Do not allow copy or assignment
	ApexClient(const ApexClient&);
	ApexClient& operator = (const ApexClient&);

private:
	CDbcReqUDPSessionRcPtr m_UDPSessionPtr;
	ACE_Reactor m_reactor;
	ACE_Thread_Manager m_threadMgr;
	ACE_thread_t m_thread_t;

};

typedef ACE_Singleton<ApexClient, ACE_Null_Mutex> CApexClient_S;
#define APEX_CLIENT_INSTANCE	CApexClient_S::instance()

#endif


