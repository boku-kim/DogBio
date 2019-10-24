#pragma once

// CClientSocket 명령 대상입니다.
#define DT_CLIENT_RECV                          WM_USER + 5   ///< send the data to dialog???????
#define DT_CLIENT_CLOSE                         WM_USER + 6

#define DT_TIMEOUT_TIMER_ID                     10

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();

    CWinThread *m_pThread;

    void SetWnd(HWND hWnd);
    HWND m_hWnd;
    virtual void OnReceive(int nErrorCode);
    virtual void OnClose(int nErrorCode);
    virtual BOOL OnMessagePending();

    BOOL SetTimeout(UINT uTimeout);
    BOOL KillTimeout();

private:
    int m_nTimerId;
};


