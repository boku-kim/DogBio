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
	//ON_EN_CHANGE(IDC_EDIT2, &UserPassword::OnEnChangeEdit2)
	//ON_EN_CHANGE(IDC_EDIT1, &UserPassword::OnEnChangeEdit1)
END_MESSAGE_MAP()


// UserPassword 메시지 처리기


void UserPassword::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
	///< get password1
	m_editPassword1.GetWindowTextW(m_strPassword);
	///< get password2
	m_editPassword2.GetWindowTextW(m_strPasswordcheck);

	//TODO: PW의 Arg 유효성 점검(String Max 길이,유효하지 못한 문자 포함 여부??)
	if (m_strPassword.GetLength() > 6 && m_strPassword.GetLength() < 11)
	{
		ps1 = 1;
		if (m_strPasswordcheck.GetLength() > 6 && m_strPasswordcheck.GetLength() < 11)
		{
			ps2 = 1;
			///< compare password1 with password2
			if (m_strPassword == m_strPasswordcheck) 
			{
				CheckPW(m_strPassword, m_strPasswordcheck);
			}
			if (ps3 == 1) 
			{
				CDialogEx::OnOK();
				MainDialog maindlg;
				maindlg.DoModal();
			}
			else if (m_strPassword != m_strPasswordcheck)
			{
				MessageBox(_T("Password가 서로 다릅니다."), _T("Password Valid"), MB_ICONERROR);
			}
		}
		else
		{
			MessageBox(_T("Password 길이는 7~10 이어야 합니다."), _T("Password2 Valid"), MB_ICONERROR);
		}
	}
	else
	{
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
CString UserPassword::GetPassword()
{
	return m_strPassword;
}