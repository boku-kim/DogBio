// EditPassword.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "EditPassword.h"
#include "afxdialogex.h"


// EditPassword 대화 상자

IMPLEMENT_DYNAMIC(EditPassword, CDialogEx)

EditPassword::EditPassword(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EDITPASSWORD, pParent)
{

}

EditPassword::~EditPassword()
{
}

void EditPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EditPassword, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CHECK, &EditPassword::OnBnClickedBtnCheck)
	ON_BN_CLICKED(IDC_BTN_CHANGE, &EditPassword::OnBnClickedBtnChange)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &EditPassword::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// EditPassword 메시지 처리기


void EditPassword::OnBnClickedBtnCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	checkpw = new CheckPassword();
	checkpw->Create(IDD_DIALOG_CHECKPASSWORD, this);
	checkpw->ShowWindow(SW_SHOW);
}


void EditPassword::OnBnClickedBtnChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	changepw = new ChangePassword();
	changepw->Create(IDD_DIALOG_CHANGEPASSWORD, this);
	changepw->ShowWindow(SW_SHOW);
}


void EditPassword::OnBnClickedBtnCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage(WM_CLOSE, 0, 0);
}
