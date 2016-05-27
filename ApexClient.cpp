#include <iostream>
#include "ApexClient.h"

using namespace std;

ApexClient::ApexClient()
{

}

ApexClient::ApexClient(const ApexClient&)
{

}

ApexClient& ApexClient::operator=(const ApexClient&)
{
	return *this;
}

ApexClient::~ApexClient()
{

}

HRESULT ApexClient::Init(USHORT ListenPort /*= 0*/)
{
	ACE_NEW_RETURN(m_UDPSessionPtr, CDbcReqUDPSession(&m_reactor, ListenPort), E_OUTOFMEMORY);

	if (S_OK != m_UDPSessionPtr->Init())
	{
		cout << "[ERROR] Failed to open local UDP port to listen" << endl;
		return E_FAIL;
	}

	if (m_threadMgr.spawn(ThreadProc, this, 0, &m_thread_t) == -1)
		return E_FAIL;

	return S_OK;
}

HRESULT ApexClient::Close()
{
	cout << "[TRACE] ApexClient::Close  calcel thread ..." << endl;
	m_threadMgr.cancel(m_thread_t);
	m_UDPSessionPtr->stop();
	Release();
	cout << "[TRACE] ApexClient::Close  udp listen thread closed!" << endl;
	return S_OK;
}

HRESULT ApexClient::Release()
{
	m_reactor.close();

	return S_OK;
}

ACE_THR_FUNC_RETURN ApexClient::ThreadProc(void* pParam)
{
	if (pParam == NULL)
		return 0;

	cout << "[TRACE] ApexClient::ThreadProc  spawn thread ..." << endl;

	ApexClient* pThis = (ApexClient*)(pParam);
	ACE_THR_FUNC_RETURN CallThisFunction = 0;

	CallThisFunction = pThis->RunThreadLoop();

	return CallThisFunction;
}

ACE_THR_FUNC_RETURN ApexClient::RunThreadLoop()
{
	cout << "[TRACE] ApexClient::RunThreadLoop" << endl;
	m_UDPSessionPtr->run();

	return 0;
}

