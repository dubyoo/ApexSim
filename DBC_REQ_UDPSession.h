#ifndef __DBC_REQ_UDP_SESSION_H__
#define __DBC_REQ_UDP_SESSION_H__

#include <string>
#include "ace/SOCK_Dgram.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Singleton.h"
#include "BBNTypes.h"
#include "ActiveQueue.h"

#define MAX_DATA_SIZE 1024

struct RawMsg
{
	BYTE m_readBuff[MAX_DATA_SIZE];
	ACE_INET_Addr m_remoteAddr;
};
typedef ACE_Refcounted_Auto_Ptr<RawMsg, ACE_Recursive_Thread_Mutex> CRawMsgRcPtr;

class CDbcReqUDPSession : public ACE_Event_Handler
{
public:
	CDbcReqUDPSession(ACE_Reactor* reactor, USHORT listenPort = 0);
	~CDbcReqUDPSession();

	HRESULT Init();

	virtual ACE_HANDLE get_handle(void) const;

	virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

	HRESULT run();
	HRESULT stop();

protected:
	HRESULT OpenUDPSock();

private:
	ACE_SOCK_Dgram  m_sock;
	USHORT		m_listenPort;
};
typedef ACE_Refcounted_Auto_Ptr<CDbcReqUDPSession, ACE_Recursive_Thread_Mutex> CDbcReqUDPSessionRcPtr;

#endif

