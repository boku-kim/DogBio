// DogRegister.cpp: 구현 파일
//
#include "stdafx.h"
#include "DogIdentification.h"
#include "DogRegister.h"
#include "afxdialogex.h"

#include <iostream>

using namespace cv;
using namespace std;

// DogRegister 대화 상자

IMPLEMENT_DYNAMIC(DogRegister, CDialogEx)

DogRegister::DogRegister(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DOGREGISTER, pParent)
	, m_radio(0)
{

}

DogRegister::~DogRegister()
{
}

void DogRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DOGNAME, m_editDogname);
	DDX_Control(pDX, IDC_PICDOGIMG, m_picDog);
	DDX_Control(pDX, IDC_EDITDOGAGE, m_editDogAge);
	DDX_Control(pDX, IDC_EDITDOGSPECIES, m_editSpecies);
	DDX_Control(pDX, IDC_EDITDOGADDRESS, m_editAddress);
	DDX_Radio(pDX, IDC_RADIO1, (int&)m_radio);
}


BEGIN_MESSAGE_MAP(DogRegister, CDialogEx)
	ON_BN_CLICKED(IDOK, &DogRegister::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTNLOADIMG, &DogRegister::OnBnClickedBtnloadimg)
	ON_BN_CLICKED(IDC_BTNOPENCAMERA, &DogRegister::OnBnClickedBtnopencamera)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, RadioCtrl)
	ON_BN_CLICKED(IDC_BTNANALYSIS, &DogRegister::OnBnClickedBtnanalysis)
END_MESSAGE_MAP()


// DogRegister 메시지 처리기


void DogRegister::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_editDogname.GetWindowTextW(m_dogName);
	m_editDogAge.GetWindowTextW(m_dogAge);
	m_editSpecies.GetWindowTextW(m_dogSpecies);
	m_editAddress.GetWindowTextW(m_dogAddr);
	if (Checkarg()) {
		CDialogEx::OnOK();
	}
}

void DogRegister::OnBnClickedBtnloadimg()
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

void DogRegister::OnBnClickedBtnopencamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CameraMgr* pCameraInst = CameraMgr::GetInstance();
	//pCameraInst->m_picDog = &(this->m_picDog);
	//pCameraInst->DoModal();
}
void DogRegister::RadioCtrl(UINT ID) {
	UpdateData(TRUE);
	switch (m_radio)
	{
	case 0:
	
		break;
	case 1:
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


void DogRegister::OnBnClickedBtnanalysis()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
