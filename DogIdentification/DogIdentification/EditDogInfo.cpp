// EditDogInfo.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "EditDogInfo.h"
#include "afxdialogex.h"


// EditDogInfo 대화 상자

IMPLEMENT_DYNAMIC(EditDogInfo, CDialogEx)

EditDogInfo::EditDogInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EDITDOGINFO, pParent)
{

}

EditDogInfo::~EditDogInfo()
{
}

void EditDogInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EditDogInfo, CDialogEx)
END_MESSAGE_MAP()


// EditDogInfo 메시지 처리기
