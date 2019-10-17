// UserPassword.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TestCode.h"
#include "UserPassword.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// UserPassword 메시지 처리기입니다.
CString UserPassword::GetPassword()
{
    return m_strPassword;
}

void UserPassword::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CDialogEx::OnOK();
    ///< get password1
    ///< get password2
    ///< compare password1 with password2
    m_editPassword1.GetWindowTextA(m_strPassword);

}
