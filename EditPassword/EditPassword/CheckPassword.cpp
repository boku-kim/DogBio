// CheckPassword.cpp: 구현 파일
//

#include "pch.h"
#include "EditPassword.h"
#include "CheckPassword.h"
#include "afxdialogex.h"


// CheckPassword 대화 상자

IMPLEMENT_DYNAMIC(CheckPassword, CDialog)

CheckPassword::CheckPassword(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CHECKPASSWORD_DIALOG, pParent)
{

}

CheckPassword::~CheckPassword()
{
}

void CheckPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CheckPassword, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CheckPassword::OnBnClickedButton1)
END_MESSAGE_MAP()


// CheckPassword 메시지 처리기


void CheckPassword::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage(WM_CLOSE, 0, 0);
}
