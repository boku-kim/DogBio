// DogRegister.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "DogRegister.h"
#include "afxdialogex.h"
#include "DbAccess.h"

#include <io.h>
#include <fstream>


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
    if (m_bmpBitmap.IsNull() != true)
    {
        m_bmpBitmap.Destroy();
    }
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
		CDC* pDc = NULL;
		pDc = m_picDog.GetWindowDC();
		CDC memdc;

        int width = 0;
        int height = 0;

		
		m_bmpBitmap.Load(img_path); // error check
		width = m_bmpBitmap.GetWidth();
		height = m_bmpBitmap.GetHeight();
		memdc.CreateCompatibleDC(pDc);
		m_bmpBitmap.TransparentBlt(pDc->m_hDC, 0, 0, iwidth, iheight, SRCCOPY);
		m_picDog.ReleaseDC(pDc);
		pDc->DeleteDC();
		pDc = NULL;

	}
    else
    {
        AfxMessageBox("failed.");
    }
}


void DogRegister::OnBnClickedBtnOpencamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CameraMgr* pCameraInst = CameraMgr::GetInstance();
	pCameraInst->m_picDog = &(this->m_picDog);
	pCameraInst->save_img = &m_bmpBitmap;

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
	m_editAge.GetWindowTextA(m_dogAge);
	m_editSpecies.GetWindowTextA(m_dogSpecies);
	m_editAddr.GetWindowTextA(m_dogAddr);
	if (Checkarg()) {

		CString file_name;
		CString path_name;
		path_name.Format("%s\\%s", SAVE_PATH, IMAGE_PATH);

		int nResult = _access(path_name, 0);  //디렉토리 있으면 0, 없으면 -1반환
		if (nResult) //없으면 dir 생성
		{
            ///< Note : MessageBox의 첫번째 패러미터는 완성된 문자열이 들어가며,
            ///< 이전에 설정한 것과 같이 printf와 같은 형식이 들어갈 수 없습니다.
            ///< 항상 절대 절대 compile warning을 주의해야 합니다.
            ///< 실무에서 compiling 도중 error가 발생한 것은 오히려 명확하기 때문에 문제파악이 쉽게 되는데 반해,
            ///< warning을 무시하게 되면, 나중에 알수 없는 에러 등으로 문제 원인 파악이 힘든 경우가 아주 많습니다.
            ///< 따라서 Compile 혹은 Link 도중 warning이 발생하면 그 원인을 파악하고 되도록이면 warning이 하나도 안나오는 상태가
            ///< 가장 이상적입니다.
            CString message = "we make folder in " + path_name;
			MessageBox(message, _T("Save Complete"), MB_OK);
			CreateDirectory((LPCTSTR)path_name, NULL);
		}

		file_name.Format("%s\\%s.jpeg", path_name, m_dogName);
		int fResult = _access(file_name, 0);//파일 있으면 0, 없으면 -1반환
		int f_num = 1;
		if (!fResult) //파일 있으면 이름에 숫자 붙이기
		{
					
			while (1)
			{
				//s_dogName.Format("%s_%d", m_dogName, f_num);
				file_name.Format("%s\\%s_%d.jpeg", path_name, m_dogName,f_num);
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

/*
** m_dogAge에 숫자가 아닌 수를 넣으면 에러
** 1~30까지만 가능
*/
int DogRegister::Checkarg() {

	int ret = 1;

	if (m_dogName == "")
	{
		MessageBox(_T("Write Dog Name"), _T("Wrong Name value"), MB_ICONWARNING);
		ret = 0;
	}

	else if (m_dogAge==""||_ttoi(m_dogAge) > 30 || _ttoi(m_dogAge) <= 0) 
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

