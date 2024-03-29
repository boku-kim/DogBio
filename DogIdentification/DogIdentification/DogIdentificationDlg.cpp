﻿
// DogIdentificationDlg.cpp: 구현 파일
//

#include"stdafx.h"
#include "DogIdentification.h"
#include "DogIdentificationDlg.h"
#include "afxdialogex.h"

#include "my_system.h"

#include "Util.h"
#include "DogRegister.h"
#include "UserPassword.h"
#include "AccessMgr.h"
#include "Login.h"
#include "EditPassword.h"
#include "SearchDogInfo.h"
#include <io.h>

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



// CDogIdentificationDlg 대화 상자
CDogIdentificationDlg::CDogIdentificationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DOGIDENTIFICATION_DIALOG, pParent)
    , m_bAlreadySetPasswd(false)
    , m_currentPath("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDogIdentificationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDogIdentificationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EDITPW, &CDogIdentificationDlg::OnBnClickedBtnEditpw)
	ON_BN_CLICKED(IDC_BTN_REGISTERDOGINFO, &CDogIdentificationDlg::OnBnClickedBtnRegisterdoginfo)
	ON_BN_CLICKED(IDC_BTN_SEARCHDOGINFO, &CDogIdentificationDlg::OnBnClickedBtnSearchdoginfo)
	ON_BN_CLICKED(IDOK, &CDogIdentificationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDogIdentificationDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDogIdentificationDlg 메시지 처리기

BOOL CDogIdentificationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

    DecideCurrentModulePath(m_currentPath);



	int nResult = _access(SAVE_PATH, 0);
	if (nResult==-1)
	{
		CreateDirectory((LPCTSTR)SAVE_PATH, NULL);
	}
	

	 /*Util* instUtil = new Util();
	delete instUtil;*/
    int nSuccessProcessPasswd = 0;
	if (TestFunction01() == eMOD_ERROR_SUCCESS)
	{
		Login login_dlg;
		login_dlg.DoModal();
		///< input password
	}
	else
	{
		///< register password -> open Register password dialog
		///< UserPassword dlgPass;
		///< dlgPass.sdfsd();
		///< dlgPass.password
        
		UserPassword dlg;
        INT_PTR nRet = -1;
        int nPasswdSuccess = 0;
        nRet = dlg.DoModal();
        if (nRet == IDOK)
        {
            nPasswdSuccess = dlg.GetPasswdSuccess();
            if (nPasswdSuccess == 1)
            {
                CString strPassword = dlg.GetPassword();

                //const char pconstUserPassword[] = "1234567890";
                AccessMgr* pAccessInst = AccessMgr::GetInstance();
                if (pAccessInst)
                {
                    pAccessInst->RegisterPassword((LPSTR)(LPCTSTR)strPassword, eSECURITY_TYPE_IMPORTANT);
                    nSuccessProcessPasswd = 1;
                }
            } 
        }
	}

    if (nSuccessProcessPasswd == 0)
    {
    }

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDogIdentificationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDogIdentificationDlg::OnPaint()
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
HCURSOR CDogIdentificationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int GetTextFromResource(int nResourceId)
{
#ifdef _LANG_KO

#endif // _LANG_KO

#ifdef _LANG_EN
#endif // _LANG_EN

	return 0;
}
eModError CDogIdentificationDlg::TestFunction01() {
	Util* pUtilInst = NULL;
	eModError ret = eMOD_ERROR_FAIL;

	pUtilInst = Util::GetInstance();
	if (pUtilInst)
	{
		bool bExist = pUtilInst->ExistFile(MY_PASSWORD_FILE_PATH);
		if (bExist == true)
		{
			m_bAlreadySetPasswd = true;
			ret = eMOD_ERROR_SUCCESS;
		}
		else
		{
			m_bAlreadySetPasswd = false;
			ret = eMOD_ERROR_FAIL;
		}
	}

	return ret;
}

///< Get current module's path
void CDogIdentificationDlg::DecideCurrentModulePath(CString& strPath)
{
    char szPath[MAX_PATH] = { 0 };
    char chDelimiter = '\\';
    int bytes = GetModuleFileName(NULL, szPath, MAX_PATH);
    if (bytes == 0)
    {
        return;
    }
    strPath.Format("%s", szPath);
    int full_len = strPath.GetLength();
    int pos = strPath.ReverseFind(chDelimiter);
    ++pos; ///< including delimiter(\)
    strPath = strPath.Left(full_len - (full_len - pos));

    OutputDebugString((LPTSTR)(LPCTSTR)strPath);
    OutputDebugString("\n");
}

void CDogIdentificationDlg::GetCurrentModulePath(CString& strPath)
{
    strPath = m_currentPath;
}



void CDogIdentificationDlg::OnBnClickedBtnEditpw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EditPassword dlgEditPassword;
	dlgEditPassword.DoModal();
}


void CDogIdentificationDlg::OnBnClickedBtnRegisterdoginfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DogRegister dlgDogRegister;
	dlgDogRegister.DoModal();
}


void CDogIdentificationDlg::OnBnClickedBtnSearchdoginfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SearchDogInfo dlgSearchDogInfo;
	dlgSearchDogInfo.DoModal();
}


void CDogIdentificationDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDogIdentificationDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CDialogEx::OnCancel();
}
