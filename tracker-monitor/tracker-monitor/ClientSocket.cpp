// ClientSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "tracker-monitor.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
    : m_pThread(NULL)
    , m_nTimerId(0)
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket ��� �Լ�
void CClientSocket::SetWnd(HWND hWnd)
{
    m_hWnd = hWnd;
}

void CClientSocket::OnReceive(int nErrorCode)
{
    // TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
    /*char szBuffer[10240];

    memset(szBuffer, 0x00, sizeof(szBuffer));

    if (Receive(szBuffer, sizeof(szBuffer)) > 0) 
    {
        SendMessage(m_hWnd, DT_CLIENT_RECV, 0, (LPARAM)szBuffer);
    }*/

    CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
    // TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
    //ShutDown();
    Close();

    if (m_pThread)
    {
        m_pThread->PostThreadMessage(WM_QUIT, 0, 0);
    }
    

    //PostMessage(m_hWnd, DT_CLIENT_CLOSE, 0, (LPARAM)this);

    CSocket::OnClose(nErrorCode);
}

BOOL CClientSocket::OnMessagePending()
{
    // TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
    MSG Message;
    if (::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
    {
        if (Message.wParam == (UINT)m_nTimerId)
        {
            ::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
            CancelBlockingCall();
            return FALSE; // No need for idle time processing.
        }
    }
    return CSocket::OnMessagePending();
}

BOOL CClientSocket::SetTimeout(UINT uTimeout)
{
    m_nTimerId = SetTimer(NULL, 0, uTimeout, NULL);
    return m_nTimerId;
}

BOOL CClientSocket::KillTimeout()
{
    return KillTimer(NULL, m_nTimerId);
}
/*
CTimeOutSocket sockServer;
CAcceptedSocket sockAccept;

sockServer.Create(777);
sockServer.Listen();

// Note the following sequence:
//  SetTimeOut
//  <operation which might block>
//  KillTimeOut

if(!sockServer.SetTimeOut(10000))
{
ASSERT(FALSE);
// Error Handling...for some reason, we could not setup
// the timer.
}

if(!sockServer.Accept(sockAccept))
{
int nError = GetLastError();
if(nError==WSAEINTR)
AfxMessageBox("No Connections Arrived For 10 Seconds");
else
; // Do other error processing.
}

if(!sockServer.KillTimeOut())
{
ASSERT(FALSE);
// Error Handling...for some reason the timer could not
// be destroyed...perhaps a memory overwrite has changed
// m_nTimerID?
//
}
...

Create, Connect, Accept, Receive �� �۾� �������� SetTimer�� �����ϰ�
������ ���� KillTimer �� ���� ���Ѿ� �Ѵ�.

���� �۾��� KillTimer�� ���ؼ� ����������
�����ڵ� WSAEINTR ���� Ȯ�� �ϸ� �ȴ�.

WSAEINTR 10004 Interrupted system call.

*/