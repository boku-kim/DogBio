// C:\for_me\dogdog191002\dogdog191002\source\source\TestDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "dogdog191002.h"
#include "C:\for_me\dogdog191002\dogdog191002\source\inc\TestDlg.h"
#include "afxdialogex.h"


// CTestDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEST_DLG, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestDlg 메시지 처리기입니다.
