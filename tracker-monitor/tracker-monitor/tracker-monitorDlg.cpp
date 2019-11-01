
// tracker-monitorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "tracker-monitor.h"
#include "tracker-monitorDlg.h"
#include "afxdialogex.h"

#include "SocketThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtrackermonitorDlg ��ȭ ����


CtrackermonitorDlg::CtrackermonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRACKERMONITOR_DIALOG, pParent)
    , m_socket()
    , m_socket_status(SOCKET_STATUS_NOT_CONNECTED)
    , m_bConnected(false)
    //, m_bLoop(false)
    , m_pThread(NULL)
    , m_strAddress("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtrackermonitorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_HELP, m_staticHelp);
    DDX_Control(pDX, IDC_BUTTON_COMM_START, m_btnCommStart);
    DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_serverAddress);
    DDX_Control(pDX, IDC_BUTTON_COMM_STOP, m_btnCommStop);
    DDX_Control(pDX, IDOK, m_btnOk);
}

BEGIN_MESSAGE_MAP(CtrackermonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_COMM_START, &CtrackermonitorDlg::OnBnClickedButtonCommStart)
    ON_BN_CLICKED(IDC_BUTTON_COMM_STOP, &CtrackermonitorDlg::OnBnClickedButtonCommStop)
    ON_MESSAGE(DT_RECEIVED_MESSAGE, &CtrackermonitorDlg::OnReceivedMessage)
    ON_MESSAGE(DT_UPDATE_HELP, &CtrackermonitorDlg::OnUpdateHelp)
    ON_MESSAGE(DT_UPDATE_CONTROL, &CtrackermonitorDlg::OnUpdateControl)
    ON_MESSAGE(DT_UPDATE_LOG, &CtrackermonitorDlg::OnUpdateLog)
    ON_MESSAGE(DT_CLIENT_RECV, &CtrackermonitorDlg::OnClientRecv)
    ON_BN_CLICKED(IDOK, &CtrackermonitorDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CtrackermonitorDlg �޽��� ó����

BOOL CtrackermonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
    m_staticHelp.SetWindowText("��� ������ ���� �ʾҽ��ϴ�.");
    m_socket_status = SOCKET_STATUS_NOT_CONNECTED;
    m_serverAddress.EnableWindow(TRUE);
    m_btnCommStart.EnableWindow(TRUE);
    m_btnCommStop.EnableWindow(FALSE);

    m_serverAddress.SetAddress(192, 168, 153, 99);
    
    m_ClientSocket.SetWnd(this->m_hWnd);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CtrackermonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CtrackermonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CtrackermonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CtrackermonitorDlg::ConnectToServer()
{
    if ((m_bConnected == false) &&
        (m_socket_status == SOCKET_STATUS_NOT_CONNECTED))
    {
        m_bConnected = true;
        m_socket_status = SOCKET_STATUS_CONNECTED;

        m_btnCommStart.EnableWindow(FALSE);
        m_btnCommStop.EnableWindow(TRUE);
        m_btnOk.EnableWindow(FALSE);

        m_socket.Create();
		int iConnectErrorCheck = -1;
		iConnectErrorCheck = m_socket.Connect(m_strAddress, DT_MONITOR_SVR_SOCKET_PORT);
        if (iConnectErrorCheck == FALSE)
        {
			CString str;
			str.Format(_T("ERROR : Failed to connect Server : %d"), iConnectErrorCheck);
            m_bConnected = false;
            m_socket_status = SOCKET_STATUS_NOT_CONNECTED;
            m_socket.Close();

            m_btnCommStart.EnableWindow(TRUE);
            m_btnCommStop.EnableWindow(FALSE);
            m_btnOk.EnableWindow(TRUE);
            return 1;
        }

        CString message = "��� ������  ���������� ���� �Ǿ����ϴ�.";
        m_staticHelp.SetWindowText(message);
        OutputDebugString(message);
    }
    return 0;
}

int CtrackermonitorDlg::DisConnectFromServer()
{
    if ((m_bConnected == true) &&
        (m_socket_status == SOCKET_STATUS_CONNECTED))
    {
        m_btnCommStart.EnableWindow(TRUE);
        m_btnCommStop.EnableWindow(FALSE);
        m_btnOk.EnableWindow(TRUE);

        //m_socket.Close();
        m_bConnected = false;
        m_socket_status = SOCKET_STATUS_NOT_CONNECTED;

        if (m_pThread)
        {
            m_pThread->PostThreadMessage(DT_STOP_MESSAGE_EXCHANGE, 0, 0);
            m_pThread->PostThreadMessage(WM_QUIT, 0, 0);
            WaitCursor(2000);
            //CloseHandle(m_pThread);
            m_pThread = NULL;
        }
        
        CString message = "��� ������  ���� �Ǿ����ϴ�.";
        m_staticHelp.SetWindowText(message);
        OutputDebugString(message);
    }   

    return 0;
}

void CtrackermonitorDlg::PostLog(CString msg)
{
    char* pMsg = NULL;
    int msg_size = 0;

    msg_size = msg.GetLength();
    pMsg = (char*)malloc(msg_size + 1);
    if (pMsg)
    {
        sprintf_s(pMsg, msg_size + 1, "%s", (LPTSTR)(LPCTSTR)msg);
        PostMessage(DT_UPDATE_LOG, (WPARAM)0, (LPARAM)pMsg);
    }
}

void CtrackermonitorDlg::OnBnClickedButtonCommStart()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    BYTE ipFirst = 0x00;
    BYTE ipSecond = 0x00;
    BYTE ipThird = 0x00;
    BYTE ipForth = 0x00;
    int ret = 0;

    m_serverAddress.GetAddress(ipFirst, ipSecond, ipThird, ipForth);
    m_strAddress.Format("%d.%d.%d.%d", ipFirst, ipSecond, ipThird, ipForth);

    ///< connect to server
    ret = ConnectToServer();
    if (ret != 0)
    {
        m_btnCommStart.EnableWindow(TRUE);
        m_btnCommStop.EnableWindow(FALSE);
        CString message = "��� ������  �����Ǿ����ϴ�.";
        m_staticHelp.SetWindowText(message);
        OutputDebugString(message);
        return;
    }

    m_pThread = (CSocketThread *)AfxBeginThread(RUNTIME_CLASS(CSocketThread),
        THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    m_pThread->SetWnd(m_hWnd);

    if (!m_pThread)
    {
        m_socket.Close();
        AfxMessageBox(_T("ERROR : Thread could not be created."));
        return;
    }

    //����Ǹ� Ŭ���̾�Ʈ�ʿ� �޽��� ����
    //m_socket.Send(_T("Connected with Server"), 43);
    // ������ ������ �и��ؼ� ������ �ڵ��� �����忡�� �Ѱ��ش�
    m_pThread->m_hSocket = m_socket.Detach();
    // ������ �����带 �����Ų��
    m_pThread->ResumeThread();

    WaitCursor(2000);

    if (m_pThread)
    {
        m_pThread->PostThreadMessage(DT_START_MESSAGE_EXCHANGE, 0, 0);
    }

}

void CtrackermonitorDlg::OnBnClickedButtonCommStop()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    DisConnectFromServer();
    
}

void CtrackermonitorDlg::WaitCursor(DWORD dwMillisecond)
{
    MSG msg;
    DWORD dwStart;
    
    dwStart = GetTickCount();

    while ((GetTickCount() - dwStart) < dwMillisecond)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

///< NOTE : if you wanna handle data, you have to do just here.
static void FreeInfoDataWrap(DtDataWrap_t** pWrap)
{
    DtDetectedArea_t* pTemp = NULL;
    DtDetectedArea_t* pTarget = NULL;

    if (*pWrap)
    {
        pTarget = (*pWrap)->detected_info;
        while (pTarget != NULL)
        {
            pTemp = pTarget->pNext;
            DT_FREE(pTarget);
            pTarget = pTemp;
        }
        DT_FREE(*pWrap);
    }
}
///< NOTE : if you wanna handle data, you have to do just here.
LRESULT CtrackermonitorDlg::OnReceivedMessage(WPARAM wParam, LPARAM lParam)
{
    DtDataWrap_t* pPassedData = NULL;
    char* pszText = NULL;
    if (lParam)
    {
        pPassedData = (DtDataWrap_t*)lParam;
		
        FreeInfoDataWrap(&pPassedData);
    }
   
    return 0;
}

LRESULT CtrackermonitorDlg::OnUpdateHelp(WPARAM wParam, LPARAM lParam)
{
    eSocketStatus status = (eSocketStatus)wParam;

    switch (status)
    {
    case SOCKET_STATUS_NOT_CONNECTED:
        m_staticHelp.SetWindowText("��� ������ �����Ǿ����ϴ�.");
        break;
    case SOCKET_STATUS_CONNECTED:
        m_staticHelp.SetWindowText("��� ������ �Ǿ����ϴ�.");
        break;
    default:
        break;
    }
    return 0;
}

LRESULT CtrackermonitorDlg::OnUpdateControl(WPARAM wParam, LPARAM lParam)
{
    eSocketStatus status = (eSocketStatus)wParam;

    switch (status)
    {
    case SOCKET_STATUS_NOT_CONNECTED:
        m_btnCommStart.EnableWindow(TRUE);
        m_btnCommStop.EnableWindow(FALSE);
        break;
    case SOCKET_STATUS_CONNECTED:
        m_btnCommStart.EnableWindow(FALSE);
        m_btnCommStop.EnableWindow(TRUE);
        break;
    default:
        break;
    }
    return 0;
}

LRESULT CtrackermonitorDlg::OnUpdateLog(WPARAM wParam, LPARAM lParam)
{
    if (lParam)
    {
        char* pMsg = (char*)lParam;
        OutputDebugString(pMsg);
        free(pMsg);
    }
    return 0;
}



afx_msg LRESULT CtrackermonitorDlg::OnClientRecv(WPARAM wParam, LPARAM lParam)
{
    //LPCTSTR lpszStr = (LPCTSTR)lParam;

    OutputDebugString("socket message was received");
    return 0;
}

//void CtrackermonitorDlg::OnBnClickedButtonGetMsg()
//{
//    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//    //*
//    if (m_pThread)
//    {
//        m_pThread->PostThreadMessage(DT_START_MESSAGE_EXCHANGE, 0, 0);
//    }
//    //*/
//}


void CtrackermonitorDlg::OnBnClickedOk()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CDialogEx::OnOK();
}
