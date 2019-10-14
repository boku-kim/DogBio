// UserPassword.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "UserPassword.h"
#include "afxdialogex.h"
#include "MainDialog.h"

// UserPassword 대화 상자

IMPLEMENT_DYNAMIC(UserPassword, CDialogEx)

UserPassword::UserPassword(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PASSWORD, pParent)
{

}

UserPassword::~UserPassword()
{
}

void UserPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editPassword1);
	DDX_Control(pDX, IDC_EDIT2, m_editPassword2);
}


BEGIN_MESSAGE_MAP(UserPassword, CDialogEx)
	ON_BN_CLICKED(IDOK, &UserPassword::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &UserPassword::OnBnClickedCancel)
END_MESSAGE_MAP()


// UserPassword 메시지 처리기


void UserPassword::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
	///< get password1

	m_editPassword1.GetWindowTextA(m_strPassword);
	//TODO: PW의 Arg 유효성 점검(String Max 길이,유효하지 못한 문자 포함 여부??)
	if (m_strPassword.GetLength() > 6 && m_strPassword.GetLength() < 11)
	{
		///< get password2
		m_editPassword2.GetWindowTextA(m_strPasswordcheck);
		if (CheckPW(m_strPassword, m_strPasswordcheck)) {
			MessageBox(_T("Password 등록"), _T("Password Check"), MB_OK);
			CDialogEx::OnOK();
			MainDialog maindlg;
			maindlg.DoModal();
		}
		else {
			MessageBox(_T("입력하신 Password가 일치하지 않습니다"), _T("Password Check"), MB_ICONERROR);
		}
		//MessageBox(_T("Password 적합"), _T("Password1 Valid"), MB_OK);
		ps1 = 1;
	}
	else {
		MessageBox(_T("Password 길이는 7~10 이어야 합니다."), _T("Password1 Valid"), MB_ICONERROR);
	}
}


void UserPassword::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

int UserPassword::CheckPW(CString str1, CString str2)
{
	//TODO: PW1, PW2 비교 
	str1 = m_strPassword;
	str2 = m_strPasswordcheck;

	if (str1.Compare(str2) == 0) {
		return 1;
	}
	else {
		return 0;
	}

}
CString UserPassword::GetPassword()
{
	return m_strPassword;
}