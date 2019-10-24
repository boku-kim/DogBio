// SearchDogInfo.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "SearchDogInfo.h"
#include "afxdialogex.h"
#include "DbAccess.h"
#include <io.h>


// SearchDogInfo 대화 상자
CString SearchDogInfo::m_dogName;
CString SearchDogInfo::m_dogAge;
CString SearchDogInfo::m_dogSpecies;
CString SearchDogInfo::m_dogAddr;
CString SearchDogInfo::m_gender;
CString SearchDogInfo::m_dogimg;
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
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
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

		// 이미지 경로 획득
		CString img_path = filedlg.GetPathName();
		CDC* pDc;
		pDc = m_PIC.GetWindowDC();
		CDC memdc;

		if (m_bmpBitmap.IsNull() != true)
		{
			m_bmpBitmap.Destroy();
		}
		//m_bmpBitmap.Destroy();
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
	pCameraInst->save_img = &m_bmpBitmap;
	pCameraInst->DoModal();
}


void SearchDogInfo::OnBnClickedBtnSearch()
{

	DbAccess::DbSearch();
	DbAccess::DbOutting();

	int nResult = _access(m_dogimg, 0);  //파일 있으면 0, 없으면 -1반환
	if (!nResult)
	{
		
		HDC dc = ::GetDC(m_PIC.m_hWnd);
		if (m_bmpBitmap.IsNull() != true)
		{
			m_bmpBitmap.Destroy();
		}
		m_bmpBitmap.Load(m_dogimg); // error check
		width = m_bmpBitmap.GetWidth();
		height = m_bmpBitmap.GetHeight();
		
		m_bmpBitmap.TransparentBlt(dc, 0, 0, iwidth, iheight, 0, 0, width, height, SRCCOPY);
		::ReleaseDC(m_PIC.m_hWnd, dc);
	}

	else
	{
		MessageBox(_T("이미지가 없습니다."), _T("No Image"), MB_OK);
	}


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

		if (Checkarg()) {

			int nResult = remove(m_dogimg);

			//m_bmpBitmap.Save(m_dogimg, Gdiplus::ImageFormatJPEG);
			m_edit_dogname.GetWindowTextA(m_dogName);
			m_editdogage.GetWindowTextA(m_dogAge);
			m_editspecies.GetWindowTextA(m_dogSpecies);
			m_editaddr.GetWindowTextA(m_dogAddr);
			CString file_name;
			file_name.Format("%s\\%s\\%s.jpeg", SAVE_PATH, IMAGE_PATH, m_dogName);
			int fResult = _access(m_dogName, 0);//파일 있으면 0, 없으면 -1반환
			int f_num = 1;
			if (!fResult) //파일 있으면 이름에 숫자 붙이기
			{


				while (1)
				{
					//s_dogName.Format("%s_%d", m_dogName, f_num);
					file_name.Format("%s\\%s\\%s_%d.jpeg", SAVE_PATH, IMAGE_PATH, m_dogName, f_num);
					fResult = _access(file_name, 0);
					if (fResult)
					{
						break;
					}
					else
					{
						f_num++;
					}

				}
			}

			m_bmpBitmap.Save(file_name, Gdiplus::ImageFormatJPEG);

			DbAccess::DbSetting(m_dogName, m_dogAge, m_gender, m_dogSpecies, m_dogAddr, file_name);
			DbAccess::DbUpdate();
			m_dogimg.Format("%s", file_name);
			MessageBox(_T("저장 완료"), _T("Save Complete"), MB_OK);

			EditOffMode();
		}
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
		int fResult = _access(m_dogimg, 0);
		if (!fResult)
		{
			int nResult = remove(m_dogimg);

			if (!nResult)
			{
				DbAccess::DbDelete();

				OnInitDialog();
				//m_PIC.SetBitmap(NULL);
				//UpdateData(FALSE);
				//CStatic* a = &m_PIC;
				//a->SetBitmap(NULL);

				m_edit_dogname.SetWindowTextA("");
				m_editdogage.SetWindowTextA("");
				m_editspecies.SetWindowTextA("");
				m_editaddr.SetWindowTextA("");

				m_btn_left.ShowWindow(0);
				m_btn_right.ShowWindow(0);

				Invalidate(true);
				MessageBox(_T("삭제 완료"), _T("Delete Complete"), MB_OK);
			}

			else
			{
				MessageBox(_T("삭제 실패"), _T("Delete Fail"), MB_OK);
			}
		}

		else
		{
			DbAccess::DbDelete();
			OnInitDialog();
			m_edit_dogname.SetWindowTextA("");
				m_editdogage.SetWindowTextA("");
				m_editspecies.SetWindowTextA("");
				m_editaddr.SetWindowTextA("");

				m_btn_left.ShowWindow(0);
				m_btn_right.ShowWindow(0);

				Invalidate(true);
				MessageBox(_T("삭제 완료"), _T("Delete Complete"), MB_OK);

		}
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
	((CButton*)GetDlgItem(IDC_RADIO_FEMALE))->SetCheck(0);

	m_btn_left.ShowWindow(0);
	m_btn_right.ShowWindow(0);

	CRect rect;
	m_PIC.GetClientRect(rect);

	iwidth = rect.Width();
	iheight = rect.Height();

	m_dogName.Format("");
	m_dogAge.Format("");
	m_dogSpecies.Format("");
	m_dogAddr.Format("");
	m_gender.Format("3");
	m_dogimg.Format("");


	EditOffMode();

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



/*
** m_dogAge에 숫자가 아닌 수를 넣으면 에러
** 1~30까지만 가능
*/
int SearchDogInfo::Checkarg() {

	int ret = 1;

	if (m_dogName == "")
	{
		MessageBox(_T("Write Dog Name"), _T("Wrong Name value"), MB_ICONWARNING);
		ret = 0;
	}

	else if (m_dogAge == "" || _ttoi(m_dogAge) > 30 || _ttoi(m_dogAge) <= 0)
	{
		MessageBox(_T("input number 1~30"), _T("Wrong Age value"), MB_ICONWARNING);
		ret = 0;

	}

	else if (m_dogSpecies == "")
	{
		MessageBox(_T("Write Species "), _T("Wrong Species value"), MB_ICONWARNING);
		ret = 0;
	}

	else if (m_dogAddr == "")
	{
		MessageBox(_T("Write Address"), _T("Wrong Address value"), MB_ICONWARNING);
		ret = 0;
	}

	return ret;

}



