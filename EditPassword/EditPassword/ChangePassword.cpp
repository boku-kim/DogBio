// ChangePassword.cpp: 구현 파일
//

#include "pch.h"
#include "EditPassword.h"
#include "ChangePassword.h"
#include "afxdialogex.h"


// ChangePassword 대화 상자

IMPLEMENT_DYNAMIC(ChangePassword, CDialog)

ChangePassword::ChangePassword(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CHANGEPASSWORD_DIALOG, pParent)
{

}

ChangePassword::~ChangePassword()
{
}

void ChangePassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChangePassword, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &ChangePassword::OnBnClickedButton1)
END_MESSAGE_MAP()


// ChangePassword 메시지 처리기


void ChangePassword::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage(WM_CLOSE, 0, 0);
}
