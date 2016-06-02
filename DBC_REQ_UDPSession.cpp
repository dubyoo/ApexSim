#include "DBC_REQ_UDPSession.h"
#include "DBC_REQ_Parser.h"
#include "ace/Event_Handler.h"
#include <iostream>

using namespace std;

CDbcReqUDPSession::CDbcReqUDPSession(ACE_Reactor* reactor, USHORT listenPort /*= 0*/)
	: ACE_Event_Handler(reactor)
	, m_listenPort(listenPort)
{
}

CDbcReqUDPSession::~CDbcReqUDPSession()
{
	m_sock.close();
}

HRESULT CDbcReqUDPSession::Init()
{
	HRESULT hr = OpenUDPSock();

	m_sock.enable(ACE_NONBLOCK);

	if (SUCCEEDED(hr))
	{
		if (reactor()->register_handler(this, ACE_Event_Handler::READ_MASK) == -1)
		{
			hr = S_FALSE;
		}
	}

	return hr;
}

ACE_HANDLE CDbcReqUDPSession::get_handle(void) const
{
	return m_sock.get_handle();
}

int CDbcReqUDPSession::handle_input(ACE_HANDLE fd /*= ACE_INVALID_HANDLE*/)
{
	cout << "[TRACE] CUDPSession::handle_input" << endl;

	CRawMsgRcPtr rawMsg;

	ACE_NEW_RETURN(rawMsg, RawMsg, -1);

	ACE_Time_Value TimeOut;
	int MSG_READ_TIMEOUT = 60 * 1000;
	TimeOut.msec(MSG_READ_TIMEOUT);
	ssize_t len = m_sock.recv(rawMsg->m_readBuff, sizeof(rawMsg->m_readBuff), rawMsg->m_remoteAddr, 0, &TimeOut);

	if (len < 0)
	{
		if ((errno != ETIME) && (errno != EAGAIN))
		{
			cout << "[ERROR] Failed to read SSP UDP Datagram, error: " << errno << endl;
		}

		return (-1);
	}

	// print 24 bytes
	cout << "[DEBUG] recv(" << rawMsg->m_remoteAddr.get_ip_address() << "):" << endl;
	for (int i = 0; i < 24; i++)
	{
		printf("%02x ", rawMsg->m_readBuff[i]);
		if (i % 4 == 3)
			printf("\n");
	}
	printf("...\n");

	// Parse DBC-REQ
	DbcReqParser parser(rawMsg->m_readBuff);
	parser.Parse();

	return 0;
}

int CDbcReqUDPSession::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK |
		ACE_Event_Handler::DONT_CALL;

	this->reactor()->remove_handler(this, mask);
	m_sock.close();

	return 0;
}

HRESULT CDbcReqUDPSession::run()
{
	cout << "[TRACE] CDbcReqUDPSession::run" << endl;
	reactor()->owner(ACE_Thread::self());
	reactor()->run_reactor_event_loop();
	return S_OK;
}

HRESULT CDbcReqUDPSession::stop()
{
	cout << "[TRACE] CDbcReqUDPSession::stop  end_reactor_event_loop ..." << endl;
	reactor()->end_reactor_event_loop();
	return S_OK;
}

HRESULT CDbcReqUDPSession::OpenUDPSock()
{
	ACE_INET_Addr ListenAddr(m_listenPort);
	m_sock.close();
	int ret = m_sock.open(ListenAddr, PF_INET, 0, 1);
	if (ret)
	{
		cout << "[ERROR] cannot listen DBC-REQ Message on port " << m_listenPort << endl;
		return S_FALSE;
	}
	cout << "[DEBUG] listen DBC-REQ message on port " << m_listenPort << endl;

	return S_OK;
}

