// EditPassword.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "DogIdentificationDlg.h"
#include "EditPassword.h"
#include "afxdialogex.h"
#include "util.h"
#include "StorageMgr.h"

// EditPassword 대화 상자

IMPLEMENT_DYNAMIC(EditPassword, CDialogEx)

EditPassword::EditPassword(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EDITPASSWORD, pParent)
{
	ischecked = false;

}

EditPassword::~EditPassword()
{
}

void EditPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_newpw1);
	DDX_Control(pDX, IDC_EDIT3, m_newpw2);
	DDX_Control(pDX, IDC_EDIT1, m_editPassword);
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
	eModError ret = eMOD_ERROR_SUCCESS;
	char szFilePath[MAX_PATH] = { 0 };
	CString strCurrentPath;
	CString strDataPath;

	CDogIdentificationDlg* pDlg = (CDogIdentificationDlg*)AfxGetMainWnd();
	pDlg->GetCurrentModulePath(strCurrentPath);
	strDataPath = strCurrentPath + "data";
	sprintf_s(szFilePath, sizeof(szFilePath), "%s\\%s", MY_PASSWORD_FILE_PATH, MY_PASSWORD_FILE_NAME);
	//MY_PASSWORD_FILE_PATH

	//sprintf_s(szFilePath, sizeof(szFilePath), "%s", MY_PASSWORD_FILE_PATH);
	m_editPassword.GetWindowTextA(m_inputPassword);
	unsigned int  data_size = m_inputPassword.GetLength();

	StorageMgr* pStorage = StorageMgr::GetInstance();
	if (pStorage)
	{
		if (data_size == 0) {
			MessageBox(_T("Password를 입력해주세요."), _T("Password Check"), MB_ICONERROR);
		}
		else {
			ret = pStorage->Compare(eSECURITY_TYPE_IMPORTANT, szFilePath, (unsigned char*)(LPTSTR)(LPCTSTR)m_inputPassword, data_size);
			if (ret == eMOD_ERROR_SUCCESS)
			{
				MessageBox(_T("확인 되었습니다.."), _T("Check Complete"), MB_OK);
				MY_CHECK_POINT("성공적으로 로그인되었습니다.");
				ischecked = true;
			}
			else {
				MessageBox(_T("입력하신 Password가 일치하지 않습니다"), _T("Password Check"), MB_ICONERROR);
			}
		}

	}
}


void EditPassword::OnBnClickedBtnChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_newpw1.GetWindowTextA(INPUT_pw1);
	m_newpw2.GetWindowTextA(INPUT_pw2);
	if (ischecked == false) {
		MessageBox(_T("Password Check를 먼저 진행해주세요."), _T("Password Check"), MB_ICONERROR);
	}
	else{
		if (INPUT_pw1.GetLength() > 6 && INPUT_pw1.GetLength() < 11)
		{
			if (INPUT_pw1 == INPUT_pw2)
			{
				MessageBox(_T("Password가 변경되었습니다."), _T("Password1 Valid"), MB_OK);
				eModError ret = eMOD_ERROR_SUCCESS;
				char szFilePath[MAX_PATH] = { 0 };
				CString strCurrentPath;
				CString strDataPath;

				CDogIdentificationDlg* pDlg = (CDogIdentificationDlg*)AfxGetMainWnd();
				pDlg->GetCurrentModulePath(strCurrentPath);
				strDataPath = strCurrentPath + "data";
				sprintf_s(szFilePath, sizeof(szFilePath), "%s\\%s", MY_PASSWORD_FILE_PATH, MY_PASSWORD_FILE_NAME);
				//sprintf_s(szFilePath, sizeof(szFilePath), "%s", MY_PASSWORD_FILE_PATH);
				StorageMgr* pStorage = StorageMgr::GetInstance();
				if (pStorage)
				{
					ret = pStorage->Store(eSECURITY_TYPE_IMPORTANT, szFilePath, (unsigned char*)(LPTSTR)(LPCTSTR)INPUT_pw1, INPUT_pw1.GetLength());
					if (ret == eMOD_ERROR_SUCCESS)
					{
						MY_CHECK_POINT("패스워드 파일이 성공적으로 저장되었습니다.");
					}
				}
				CDialogEx::OnOK();
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

}


void EditPassword::OnBnClickedBtnCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage(WM_CLOSE, 0, 0);
}
