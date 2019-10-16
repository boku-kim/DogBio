// DogRegister.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "DogRegister.h"
#include "DbAccess.h"
#include "afxdialogex.h"


// DogRegister 대화 상자

IMPLEMENT_DYNAMIC(DogRegister, CDialogEx)

DogRegister::DogRegister(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DOGREGISTER, pParent)
	, m_radiogender(0) 
{
	m_gender = "male";
}

DogRegister::~DogRegister()
{
}

void DogRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICDOGIMG, m_picDog);
	DDX_Control(pDX, IDC_EDIT_DOGNAME, m_editDogname);
	DDX_Control(pDX, IDC_EDIT_DOGAGE, m_editAge);
	DDX_Control(pDX, IDC_EDIT_DOGSPECIES, m_editSpecies);
	DDX_Control(pDX, IDC_EDIT_DOGADDR, m_editAddr);
	DDX_Radio(pDX, IDC_RADIO_MALE, (int&)m_radiogender);
	DDX_Control(pDX, IDC_BTN_ANALYSIS, m_analysis);
}


BEGIN_MESSAGE_MAP(DogRegister, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOADIMG, &DogRegister::OnBnClickedBtnLoadimg)
	ON_BN_CLICKED(IDC_BTN_OPENCAMERA, &DogRegister::OnBnClickedBtnOpencamera)
	ON_BN_CLICKED(IDC_BTN_ANALYSIS, &DogRegister::OnBnClickedBtnAnalysis)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_MALE, IDC_RADIO_FEMALE, RadioCtrl)
	ON_BN_CLICKED(IDOK, &DogRegister::OnBnClickedOk)
END_MESSAGE_MAP()


// DogRegister 메시지 처리기


void DogRegister::OnBnClickedBtnLoadimg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szFilter = _T(" All Files(*.*)|*.*|");
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (IDOK == filedlg.DoModal()) {
		//picture control 크기 획득
		CRect rect;
		m_picDog.GetClientRect(rect);
		int iwidth, iheight;
		iwidth = rect.Width();
		iheight = rect.Height();

		// 이미지 경로 획득
		CString img_path = filedlg.GetPathName();
		CDC* pDc;
		pDc = m_picDog.GetWindowDC();
		CDC memdc;

		int width, height;
		m_bmpBitmap.Destroy();
		m_bmpBitmap.Load(img_path);
		width = m_bmpBitmap.GetWidth();
		height = m_bmpBitmap.GetHeight();
		memdc.CreateCompatibleDC(pDc);
		m_bmpBitmap.TransparentBlt(pDc->m_hDC, 0, 0, iwidth, iheight, SRCCOPY);
		m_picDog.ReleaseDC(pDc);
		pDc->DeleteDC();
		pDc = NULL;

	}
}


void DogRegister::OnBnClickedBtnOpencamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CameraMgr* pCameraInst = CameraMgr::GetInstance();
	pCameraInst->m_picDog = &(this->m_picDog);
	pCameraInst->DoModal();
}


void DogRegister::OnBnClickedBtnAnalysis()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(_T("분석완료. 추가 정보를 입력해주세요"), _T("Analysis complete"), MB_OK);
}


void DogRegister::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_editDogname.GetWindowTextA(m_dogName);
	//m_editDogname.GetWindowTextA(m_dogName);
	m_editAge.GetWindowTextA(m_dogAge);
	m_editSpecies.GetWindowTextA(m_dogSpecies);
	m_editAddr.GetWindowTextA(m_dogAddr);

	

	if (Checkarg()) {

		DbAccess::DbSetting(m_dogName, m_dogAge, m_gender, m_dogSpecies, m_dogAddr);

		DbAccess::DbInsert();

		MessageBox(_T("저장되었습니다."), _T("Save Complete"), MB_OK);
		CDialogEx::OnOK();
	}
}
void DogRegister::RadioCtrl(UINT ID) {
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
int DogRegister::Checkarg() {
	if (_ttoi(m_dogAge) > 30) {
		MessageBox(_T("input 1~30"), _T("Wrong Age value"), MB_ICONWARNING);
		return 0;
	}
	return 1;
}
