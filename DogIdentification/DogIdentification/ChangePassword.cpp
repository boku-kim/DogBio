// ChangePassword.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "ChangePassword.h"
#include "afxdialogex.h"


// ChangePassword 대화 상자

IMPLEMENT_DYNAMIC(ChangePassword, CDialogEx)

ChangePassword::ChangePassword(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHANGEPASSWORD, pParent)
{

}

ChangePassword::~ChangePassword()
{
}

void ChangePassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChangePassword, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OK, &ChangePassword::OnBnClickedBtnOk)
END_MESSAGE_MAP()


// ChangePassword 메시지 처리기


void ChangePassword::OnBnClickedBtnOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage(WM_CLOSE, 0, 0);
}
