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
	DDX_Control(pDX, IDC_EDIT2, m_newpw1);
	DDX_Control(pDX, IDC_EDIT3, m_newpw2);
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

	if (1)
	{
		MessageBox(_T("확인되었습니다."), _T("Password1 Valid"), MB_OK);
	}
	//checkpw = new CheckPassword();
	//checkpw->Create(IDD_DIALOG_CHECKPASSWORD, this);
	//checkpw->ShowWindow(SW_SHOW);
}


void EditPassword::OnBnClickedBtnChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_newpw1.GetWindowTextW(INPUT_pw1);
	m_newpw2.GetWindowTextW(INPUT_pw2);

	if (INPUT_pw1.GetLength() > 6 && INPUT_pw1.GetLength() < 11)
	{
		if(INPUT_pw2.GetLength() > 6 && INPUT_pw2.GetLength() < 11)
		{
			if (INPUT_pw1 == INPUT_pw2)
			{
				MessageBox(_T("Password가 변경되었습니다."), _T("Password1 Valid"), MB_OK);
			}
			else
			{
				MessageBox(_T("Password가 서로 다릅니다."), _T("Password1 Valid"), MB_ICONERROR);
			}
		}
		else
		{
			MessageBox(_T("Password 길이는 7~10 이어야 합니다."), _T("Password1 Valid"), MB_ICONERROR);
		}
	}
	else 
	{
		MessageBox(_T("Password 길이는 7~10 이어야 합니다."), _T("Password1 Valid"), MB_ICONERROR);
	}
	//changepw = new ChangePassword();
	//changepw->Create(IDD_DIALOG_CHANGEPASSWORD, this);
	//changepw->ShowWindow(SW_SHOW);
}


void EditPassword::OnBnClickedBtnCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage(WM_CLOSE, 0, 0);
}
