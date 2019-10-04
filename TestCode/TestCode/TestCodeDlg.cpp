
// TestCodeDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TestCode.h"
#include "TestCodeDlg.h"
#include "afxdialogex.h"

#include "my_system.h"

#include "Util.h"
#include "StorageMgr.h"
#include "AccessMgr.h"
#include "UserPassword.h"

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


// CTestCodeDlg ��ȭ ����



CTestCodeDlg::CTestCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTCODE_DIALOG, pParent)
    , m_instUtil(NULL)
    , m_bAlreadySetPasswd(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestCodeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON_TEST, m_btnRunTestCode);
    DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
}

BEGIN_MESSAGE_MAP(CTestCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CTestCodeDlg::OnBnClickedButtonTest)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestCodeDlg �޽��� ó����

BOOL CTestCodeDlg::OnInitDialog()
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
    /*Util* instUtil = new Util();

    delete instUtil;*/

    if (TestFunction01() == eMOD_ERROR_SUCCESS)
    {
        ///< input password
    }
    else
    {
        ///< register password -> open Register password dialog
        ///< UserPassword dlgPass;
        ///< dlgPass.sdfsd();
        ///< dlgPass.password
        UserPassword dlg;
        dlg.DoModal();
        CString strPassword = dlg.GetPassword();

        //const char pconstUserPassword[] = "1234567890";

        AccessMgr* pAccessInst = AccessMgr::GetInstance();
        if (pAccessInst)
        {
            pAccessInst->RegisterPassword((LPSTR)(LPCTSTR)strPassword, eSECURITY_TYPE_IMPORTANT);
        }


    }

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

int GetTextFromResource(int nResourceId)
{
#ifdef _LANG_KO

#endif // _LANG_KO

#ifdef _LANG_EN
#endif // _LANG_EN

    return 0;
}

void CTestCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestCodeDlg::OnPaint()
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
HCURSOR CTestCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestCodeDlg::OnBnClickedButtonTest()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString strTemp = "";
    

    strTemp.Format("%s", BTN_CAPTION);
    m_btnRunTestCode.SetWindowText(strTemp);

    //TestFunction01();
}


eModError CTestCodeDlg::TestFunction01()
{
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

void CTestCodeDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    Util::ReleaseInstance();

    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
    /*if (m_instUtil)
    {
        delete m_instUtil;
        m_instUtil = NULL;
    }*/
}
