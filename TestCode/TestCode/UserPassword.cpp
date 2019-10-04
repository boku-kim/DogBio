// UserPassword.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TestCode.h"
#include "UserPassword.h"
#include "afxdialogex.h"
#include "StorageMgr.h"


// UserPassword 대화 상자입니다.

IMPLEMENT_DYNAMIC(UserPassword, CDialogEx)

UserPassword::UserPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PASSWORD, pParent)
{

}

UserPassword::~UserPassword()
{
}

void UserPassword::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PW1, m_editPassword1);
    DDX_Control(pDX, IDC_EDIT_PW2, m_editPassword2);
}


BEGIN_MESSAGE_MAP(UserPassword, CDialogEx)
    ON_BN_CLICKED(IDOK, &UserPassword::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &UserPassword::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_PW2, &UserPassword::OnEnChangeEditPw2)
	ON_EN_CHANGE(IDC_EDIT_PW1, &UserPassword::OnEnChangeEditPw1)
	ON_EN_CHANGE(IDC_EDIT6, &UserPassword::OnEnChangeEdit6)
	ON_STN_CLICKED(IDC_STATIC1, &UserPassword::OnStnClickedStatic1)
END_MESSAGE_MAP()


// UserPassword 메시지 처리기입니다.
CString UserPassword::GetPassword()
{
    return m_strPassword;
}

void UserPassword::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //CDialogEx::OnOK();
    ///< get password1
	
	m_editPassword1.GetWindowTextA(m_strPassword);
	//TODO: PW의 Arg 유효성 점검(String Max 길이,유효하지 못한 문자 포함 여부??)
	if (m_strPassword.GetLength() > 6 && m_strPassword.GetLength() < 11)
	{
		MessageBox(_T("Password 적합"), _T("Password1 Valid"), MB_OK);
		ps1 = 1;
	}
	else {
			MessageBox(_T("Password 길이는 7~10 이어야 합니다."), _T("Password1 Valid"), MB_ICONERROR);
			
	}
    ///< get password2
	m_editPassword2.GetWindowTextA(m_strPasswordcheck);
	if (m_strPasswordcheck.GetLength() > 6 && m_strPasswordcheck.GetLength() < 11)
	{
		MessageBox(_T("Password 적합"), _T("Password2 Valid"), MB_OK);
		ps2 = 1;
	}
	else {
			MessageBox(_T("Password 길이는 7~10 이어야 합니다."), _T("Password2 Valid"), MB_ICONERROR);
			
	}
    ///< compare password1 with password2
	if(ps1 == 1 && ps2 == 1) {
		CheckPW(m_strPassword, m_strPasswordcheck);
	}
	if (ps3 == 1) {
		CDialogEx::OnOK();
	}


	
}

int UserPassword::CheckPW(CString str1, CString str2)
{
	//TODO: PW1, PW2 비교 
	str1 = m_strPassword;
	str2 = m_strPasswordcheck;
	
	if (str1.Compare(str2) == 0) {
		//if 비교 후 동일하면 return 1, message popup 등록 가능한 PW
		MessageBox(_T("Password 등록"), _T("Password Check"), MB_OK);
		m_stors.Setm_stor1(m_strPassword); //Setm_stor1:m_stors의 stor1으로 password를 받음(store에 보낼) 
		return ps3 = 1;
	}
	else {
		//else 비교 후 다르면 return 0,message popup 입력하신 Password가 일치하지 않습니다
		MessageBox(_T("입력하신 Password가 일치하지 않습니다"), _T("Password Check"), MB_ICONERROR);
		
	}
	
}
void UserPassword::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
	//레지스트메뉴를 종료합니다
	/// close file:f.close()
	///or out from GUI
}


void UserPassword::OnEnChangeEditPw2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void UserPassword::OnEnChangeEditPw1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void UserPassword::OnEnChangeEdit6()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void UserPassword::OnStnClickedStatic1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
