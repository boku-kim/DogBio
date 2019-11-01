
// tracker-monitorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "tracker-monitor.h"
#include "tracker-monitorDlg.h"
#include "afxdialogex.h"

#include "SocketThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CtrackermonitorDlg 대화 상자


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


// CtrackermonitorDlg 메시지 처리기

BOOL CtrackermonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
    m_staticHelp.SetWindowText("통신 연결이 되지 않았습니다.");
    m_socket_status = SOCKET_STATUS_NOT_CONNECTED;
    m_serverAddress.EnableWindow(TRUE);
    m_btnCommStart.EnableWindow(TRUE);
    m_btnCommStop.EnableWindow(FALSE);

    m_serverAddress.SetAddress(192, 168, 153, 99);
    
    m_ClientSocket.SetWnd(this->m_hWnd);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CtrackermonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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

        CString message = "통신 연결이  성공적으로 진행 되었습니다.";
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
        
        CString message = "통신 연결이  해제 되었습니다.";
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
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
        CString message = "통신 연결이  해제되었습니다.";
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

    //연결되면 클라이언트쪽에 메시지 보냄
    //m_socket.Send(_T("Connected with Server"), 43);
    // 접속한 소켓을 분리해서 소켓의 핸들을 스레드에게 넘겨준다
    m_pThread->m_hSocket = m_socket.Detach();
    // 생성한 스레드를 실행시킨다
    m_pThread->ResumeThread();

    WaitCursor(2000);

    if (m_pThread)
    {
        m_pThread->PostThreadMessage(DT_START_MESSAGE_EXCHANGE, 0, 0);
    }

}

void CtrackermonitorDlg::OnBnClickedButtonCommStop()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
        m_staticHelp.SetWindowText("통신 연결이 해제되었습니다.");
        break;
    case SOCKET_STATUS_CONNECTED:
        m_staticHelp.SetWindowText("통신 연결이 되었습니다.");
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
//    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//    //*
//    if (m_pThread)
//    {
//        m_pThread->PostThreadMessage(DT_START_MESSAGE_EXCHANGE, 0, 0);
//    }
//    //*/
//}


void CtrackermonitorDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CDialogEx::OnOK();
}
