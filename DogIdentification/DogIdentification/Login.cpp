// Login.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "Login.h"
#include "afxdialogex.h"
#include "MainDialog.h"

// Login 대화 상자

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

Login::~Login()
{
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &Login::OnBnClickedBtnLogin)
END_MESSAGE_MAP()


// Login 메시지 처리기


void Login::OnBnClickedBtnLogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
