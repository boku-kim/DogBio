
// dogdog191002Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "dogdog191002.h"
#include "dogdog191002Dlg.h"
#include "afxdialogex.h"
#include "TestDlg.h"

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


// Cdogdog191002Dlg 대화 상자

//Dogdata _dog_info;


Cdogdog191002Dlg::Cdogdog191002Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DOGDOG191002_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void Cdogdog191002Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, _edit_name);
	DDX_Control(pDX, IDC_EDIT_AGE, _edit_age);
	DDX_Control(pDX, IDC_EDIT_SPECIES, _edit_species);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, _edit_address);
	DDX_Control(pDX, IDC_BUTTON_LOAD_IMG, _btn_load_image);
	DDX_Control(pDX, IDC_BUTTON_OPEN_CAMERA, _btn_open_camera);
	DDX_Control(pDX, IDC_BUTTON1, _btn_left);
	DDX_Control(pDX, IDC_BUTTON2, _btn_right);
}

BEGIN_MESSAGE_MAP(Cdogdog191002Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cdogdog191002Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cdogdog191002Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMG, &Cdogdog191002Dlg::OnBnClickedButtonLoadImg)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CAMERA, &Cdogdog191002Dlg::OnBnClickedButtonOpenCamera)

END_MESSAGE_MAP()


// Cdogdog191002Dlg 메시지 처리기

BOOL Cdogdog191002Dlg::OnInitDialog()
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
	_dog_info.name = "solsol";
	_dog_info.age = "25";
	_dog_info.address = "서울시 서대문구 가옥";
	_dog_info.species = "인간";
	e_flag = 0;
	EditOffMode();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Cdogdog191002Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cdogdog191002Dlg::OnPaint()
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
HCURSOR Cdogdog191002Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cdogdog191002Dlg::EditOnMode()
{
	//_edit_name.SetReadOnly(false);
	//_edit_age.SetReadOnly(false);
	//_edit_species.SetReadOnly(false);
	//_edit_address.SetReadOnly(false);
	_edit_name.EnableWindow(1);
	_edit_age.EnableWindow(1);
	_edit_address.EnableWindow(1);
	_edit_species.EnableWindow(1);
	_edit_name.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	_edit_age.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	_edit_species.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	_edit_address.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	_btn_load_image.ShowWindow(1);
	_btn_open_camera.ShowWindow(1);
	_edit_name.ShowWindow(1);
	_edit_age.ShowWindow(1);
	_edit_address.ShowWindow(1);
	_edit_species.ShowWindow(1);
	//GetDlgItemText(IDC_BUTTON1, "Save");
	_btn_left.SetWindowTextA((LPCTSTR)"Save");
	_btn_right.SetWindowTextA((LPCTSTR)"Cancel");
	
}
void Cdogdog191002Dlg::EditOffMode()
{
	//_edit_name.SetReadOnly(true);
	//_edit_age.SetReadOnly(true);
	//_edit_species.SetReadOnly(true);
	//_edit_address.SetReadOnly(true);
	_edit_name.SetWindowTextA((LPCTSTR)_dog_info.name);
	_edit_age.SetWindowTextA((LPCTSTR)_dog_info.age);
	_edit_address.SetWindowTextA((LPCTSTR)_dog_info.address);
	_edit_species.SetWindowTextA((LPCTSTR)_dog_info.species);
	_edit_name.EnableWindow(0);
	_edit_age.EnableWindow(0);
	_edit_address.EnableWindow(0);
	_edit_species.EnableWindow(0);
	_edit_name.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	_edit_age.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	_edit_species.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	_edit_address.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	_btn_load_image.ShowWindow(0);
	_btn_open_camera.ShowWindow(0);
	/*_btn_left.GetWindowTextW("Edit");
	_btn_left.GetWindowTextW("Delete");*/
	_btn_left.SetWindowTextA((LPCTSTR)"Edit");
	_btn_right.SetWindowTextA((LPCTSTR)"Delete");

}

void Cdogdog191002Dlg::OnBnClickedButton1()
{
	e_flag = !e_flag;

	if (e_flag)
	{
		EditOnMode();
	}

	else
	{
		GetDlgItemTextA(IDC_EDIT_NAME, _dog_info.name);
		GetDlgItemTextA(IDC_EDIT_AGE, _dog_info.age);
		GetDlgItemTextA(IDC_EDIT_SPECIES, _dog_info.species);
		GetDlgItemTextA(IDC_EDIT_ADDRESS, _dog_info.address);
		EditOffMode();
	}
	//_edit_age.adjustBorder(true);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void Cdogdog191002Dlg::OnBnClickedButton2()
{
	
	if (!e_flag)
	{
		CTestDlg tesss;
		tesss.DoModal();

	}

	else
	{
		e_flag = !e_flag;
		EditOffMode();
	}
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cdogdog191002Dlg::OnBnClickedButtonLoadImg()
{
	CTestDlg testdlg;
	testdlg.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cdogdog191002Dlg::OnBnClickedButtonOpenCamera()
{
	CTestDlg testdlg;
	testdlg.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


