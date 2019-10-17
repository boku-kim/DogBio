// MainDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "MainDialog.h"
#include "afxdialogex.h"
#include "EditPassword.h"
#include "EditDogInfo.h"
#include "DogRegister.h"
#include "SearchDogInfo.h"

// MainDialog 대화 상자

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{

}

MainDialog::~MainDialog()
{
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_EDITPW, &MainDialog::OnBnClickedBtnEditpw)
	ON_BN_CLICKED(IDC_BTN_REGISTERDOGINFO, &MainDialog::OnBnClickedBtnRegisterdoginfo)
	ON_BN_CLICKED(IDC_BTN_EDITDOGINFO, &MainDialog::OnBnClickedBtnEditdoginfo)
	ON_BN_CLICKED(IDC_BTN_SEARCHDOGINFO, &MainDialog::OnBnClickedBtnSearchdoginfo)
END_MESSAGE_MAP()


// MainDialog 메시지 처리기


void MainDialog::OnBnClickedBtnEditpw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EditPassword dlgEditPassword;
	dlgEditPassword.DoModal();
}


void MainDialog::OnBnClickedBtnRegisterdoginfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DogRegister dlgDogRegister;
	dlgDogRegister.DoModal();
}


void MainDialog::OnBnClickedBtnEditdoginfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EditDogInfo dlgEditDogInfo;
	dlgEditDogInfo.DoModal();
}


void MainDialog::OnBnClickedBtnSearchdoginfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SearchDogInfo dlgSearchDogInfo;
	dlgSearchDogInfo.DoModal();
}
