// SearchDogInfo.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "SearchDogInfo.h"
#include "afxdialogex.h"
#include "DbAccess.h"


// SearchDogInfo 대화 상자

CString SearchDogInfo::m_dogName;
CString SearchDogInfo::m_dogAge;
CString SearchDogInfo::m_dogSpecies;
CString SearchDogInfo::m_dogAddr;
CString SearchDogInfo::m_gender;

IMPLEMENT_DYNAMIC(SearchDogInfo, CDialogEx)

SearchDogInfo::SearchDogInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SEARCHDOGINFO, pParent)
	, m_radiogender(0)
{

}

SearchDogInfo::~SearchDogInfo()
{
}

void SearchDogInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICDOGIMG, m_PIC);
	DDX_Control(pDX, IDC_EDIT_DOGNAME, m_edit_dogname);
	DDX_Control(pDX, IDC_EDIT_DOGAGE, m_editdogage);
	DDX_Control(pDX, IDC_EDIT_DOGSPECIES, m_editspecies);
	DDX_Control(pDX, IDC_EDIT_DOGADDR, m_editaddr);
	DDX_Radio(pDX, IDC_RADIO_MALE, (int&)m_radiogender);
	DDX_Control(pDX, IDC_BTN_LOADIMG, m_btn_load);
	DDX_Control(pDX, IDC_BTN_OPENCAMERA, m_btn_opencam);
	DDX_Control(pDX, IDC_BTN1, m_btn_left);
	DDX_Control(pDX, IDC_BTN2, m_btn_right);
}


BEGIN_MESSAGE_MAP(SearchDogInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOADIMG, &SearchDogInfo::OnBnClickedBtnLoadimg)
	ON_BN_CLICKED(IDC_BTN_OPENCAMERA, &SearchDogInfo::OnBnClickedBtnOpencamera)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &SearchDogInfo::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN1, &SearchDogInfo::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN2, &SearchDogInfo::OnBnClickedBtn2)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_MALE, IDC_RADIO_FEMALE, RadioCtrl)
END_MESSAGE_MAP()


// SearchDogInfo 메시지 처리기

void SearchDogInfo::RadioCtrl(UINT ID) {
	UpdateData(TRUE);
	switch (m_radiogender)
	{
	case 0:
		m_gender.SetString("male");
		break;
	case 1:
		m_gender.SetString("female");
		break;
	default:
		break;
	}
}
void SearchDogInfo::OnBnClickedBtnLoadimg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szFilter = _T(" All Files(*.*)|*.*|");
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)szFilter);
	if (filedlg.DoModal() == IDOK)
	{
		//picture control 크기 획득
		CRect rect;
		m_PIC.GetClientRect(rect);
		int iwidth, iheight;
		iwidth = rect.Width();
		iheight = rect.Height();

		// 이미지 경로 획득
		CString img_path = filedlg.GetPathName();
		CDC* pDc;
		pDc = m_PIC.GetWindowDC();
		CDC memdc;

		int width, height;
		m_bmpBitmap.Destroy();
		m_bmpBitmap.Load(img_path);
		width = m_bmpBitmap.GetWidth();
		height = m_bmpBitmap.GetHeight();
		memdc.CreateCompatibleDC(pDc);
		m_bmpBitmap.TransparentBlt(pDc->m_hDC, 0, 0, iwidth, iheight, SRCCOPY);
		m_PIC.ReleaseDC(pDc);
		pDc->DeleteDC();
		pDc = NULL;
	}
}


void SearchDogInfo::OnBnClickedBtnOpencamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CameraMgr* pCameraInst = CameraMgr::GetInstance();
	pCameraInst->m_picDog = &(this->m_PIC);
	pCameraInst->DoModal();
}


void SearchDogInfo::OnBnClickedBtnSearch()
{

	DbAccess::DbSearch();
	DbAccess::DbOutting();

	m_edit_dogname.SetWindowTextA(m_dogName);
	m_editdogage.SetWindowTextA(m_dogAge);
	m_editspecies.SetWindowTextA(m_dogSpecies);
	m_editaddr.SetWindowTextA(m_dogAddr);


	if (m_gender == "male")
	{
		m_radiogender = 0;
	}

	else
	{
		m_radiogender = 1;
	}

	UpdateData(false);
	m_btn_left.ShowWindow(1);
	m_btn_right.ShowWindow(1);
	EditOffMode();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(_T("검색 완료"), _T("Search Complete"), MB_OK);
}

void SearchDogInfo::EditOnMode(){
	m_edit_dogname.EnableWindow(1);
	m_editdogage.EnableWindow(1);
	m_editspecies.EnableWindow(1);
	m_editaddr.EnableWindow(1);
	m_edit_dogname.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_editdogage.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_editspecies.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_editaddr.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_edit_dogname.ShowWindow(1);
	m_editdogage.ShowWindow(1);
	m_editspecies.ShowWindow(1);
	m_editaddr.ShowWindow(1);

	m_btn_left.SetWindowTextA("Save");
	m_btn_right.SetWindowTextA("Cancel");
	GetDlgItem(IDC_RADIO_MALE)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_FEMALE)->EnableWindow(TRUE);
}
void SearchDogInfo::EditOffMode() {
	m_edit_dogname.SetWindowTextA(m_dogName);
	m_editdogage.SetWindowTextA(m_dogAge);
	m_editspecies.SetWindowTextA(m_dogSpecies);
	m_editaddr.SetWindowTextA(m_dogAddr);

	m_edit_dogname.EnableWindow(0);
	m_editdogage.EnableWindow(0);
	m_editspecies.EnableWindow(0);
	m_editaddr.EnableWindow(0);
	m_edit_dogname.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_editdogage.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_editspecies.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_editaddr.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	//m_edit_dogname.ShowWindow(0);
	//m_editdogage.ShowWindow(0);
	//m_editspecies.ShowWindow(0);
	//m_editaddr.ShowWindow(0);
	
	m_btn_left.SetWindowTextA("Edit");
	m_btn_right.SetWindowTextA("Delete");
	GetDlgItem(IDC_RADIO_MALE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_FEMALE)->EnableWindow(FALSE);
}
void SearchDogInfo::OnBnClickedBtn1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	e_flag = !e_flag;

	if (e_flag)
	{
		EditOnMode();
	}

	else
	{
		m_edit_dogname.GetWindowTextA(m_dogName);
		m_editdogage.GetWindowTextA(m_dogAge);
		m_editspecies.GetWindowTextA(m_dogSpecies);
		m_editaddr.GetWindowTextA(m_dogAddr);

		DbAccess::DbSetting(m_dogName, m_dogAge, m_gender, m_dogSpecies, m_dogAddr);
		DbAccess::DbUpdate();

		MessageBox(_T("저장 완료"), _T("Save Complete"), MB_OK);
		EditOffMode();
	}
	//_edit_age.adjustBorder(true);
}


void SearchDogInfo::OnBnClickedBtn2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!e_flag)
	{
		/*CTestDlg tesss;
		tesss.DoModal();
		*/
		DbAccess::DbDelete();

		m_edit_dogname.SetWindowTextA("");
		m_editdogage.SetWindowTextA("");
		m_editspecies.SetWindowTextA("");
		m_editaddr.SetWindowTextA("");
		((CButton*)GetDlgItem(IDC_RADIO_MALE))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_FEMALE))->SetCheck(0);
		m_btn_left.ShowWindow(0);
		m_btn_right.ShowWindow(0);
		MessageBox(_T("삭제 완료"), _T("Delete Complete"), MB_OK);
	}


	else
	{
		e_flag = !e_flag;
		EditOffMode();
	}
}


BOOL SearchDogInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	((CButton*)GetDlgItem(IDC_RADIO_MALE))->SetCheck(0);
	m_btn_left.ShowWindow(0);
	m_btn_right.ShowWindow(0);
	EditOffMode();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
