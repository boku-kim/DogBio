
// tracker-monitorDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "ClientSocket.h"
#include "SocketThread.h"

#define DT_MONITOR_SVR_SOCKET_PORT              11111
#define DT_RECEIVED_MESSAGE                     WM_USER + 1
#define DT_UPDATE_HELP                          WM_USER + 2
#define DT_UPDATE_CONTROL                       WM_USER + 3
#define DT_UPDATE_LOG                           WM_USER + 4
//#define DT_CLIENT_RECV                          WM_USER + 5

// CtrackermonitorDlg ��ȭ ����
class CtrackermonitorDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CtrackermonitorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRACKERMONITOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
public:
    //static UINT ThreadFunction(LPVOID pData);

private:
    void WaitCursor(DWORD dwMillisecond);
    afx_msg LRESULT OnReceivedMessage(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateHelp(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateControl(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateLog(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnClientRecv(WPARAM wParam, LPARAM lParam);
    
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    CSocket m_socket;
    eSocketStatus m_socket_status;
    bool m_bConnected;
    
    CSocketThread* m_pThread;
    CString m_strAddress;
    CClientSocket m_ClientSocket;

public:
    CButton m_btnCommStart;
    CStatic m_staticHelp;
    CIPAddressCtrl m_serverAddress;
    CButton m_btnCommStop;
    afx_msg void OnBnClickedButtonCommStart();
    afx_msg void OnBnClickedButtonCommStop();

private:    
    int ConnectToServer();
    int DisConnectFromServer();
    void PostLog(CString msg);

public:
    afx_msg void OnBnClickedOk();
    CButton m_btnOk;
};
