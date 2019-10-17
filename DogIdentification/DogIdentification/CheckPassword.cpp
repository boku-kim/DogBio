// CheckPassword.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "CheckPassword.h"
#include "afxdialogex.h"


// CheckPassword 대화 상자

IMPLEMENT_DYNAMIC(CheckPassword, CDialogEx)

CheckPassword::CheckPassword(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHECKPASSWORD, pParent)
{

}

CheckPassword::~CheckPassword()
{
}

void CheckPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CheckPassword, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OK, &CheckPassword::OnBnClickedBtnOk)
END_MESSAGE_MAP()


// CheckPassword 메시지 처리기


void CheckPassword::OnBnClickedBtnOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage(WM_CLOSE, 0, 0);
}
