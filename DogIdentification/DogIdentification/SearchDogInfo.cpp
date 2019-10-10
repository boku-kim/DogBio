// SearchDogInfo.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "SearchDogInfo.h"
#include "afxdialogex.h"


// SearchDogInfo 대화 상자

IMPLEMENT_DYNAMIC(SearchDogInfo, CDialogEx)

SearchDogInfo::SearchDogInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SEARCHDOGINFO, pParent)
{

}

SearchDogInfo::~SearchDogInfo()
{
}

void SearchDogInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICDOGIMG, m_PIC);
}


BEGIN_MESSAGE_MAP(SearchDogInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOADIMG, &SearchDogInfo::OnBnClickedBtnLoadimg)
	ON_BN_CLICKED(IDC_BTN_OPENCAMERA, &SearchDogInfo::OnBnClickedBtnOpencamera)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &SearchDogInfo::OnBnClickedBtnSearch)
END_MESSAGE_MAP()


// SearchDogInfo 메시지 처리기


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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
