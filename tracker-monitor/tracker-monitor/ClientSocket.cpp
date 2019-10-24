// ClientSocket.cpp : 구현 파일입니다.
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


// CClientSocket 멤버 함수
void CClientSocket::SetWnd(HWND hWnd)
{
    m_hWnd = hWnd;
}

void CClientSocket::OnReceive(int nErrorCode)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

Create, Connect, Accept, Receive 등 작업 시작전에 SetTimer를 설정하고
끝나고 나면 KillTimer 로 해제 시켜야 한다.

소켓 작업이 KillTimer에 의해서 죽은건지는
에러코드 WSAEINTR 으로 확인 하면 된다.

WSAEINTR 10004 Interrupted system call.

*/