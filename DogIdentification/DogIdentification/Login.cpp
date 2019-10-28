// Login.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "DogIdentificationDlg.h"
#include "Login.h"
#include "afxdialogex.h"
#include "util.h"
#include "StorageMgr.h"

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
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &Login::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &Login::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// Login 메시지 처리기


void Login::OnBnClickedBtnLogin()
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
				MessageBox(_T("로그인 되었습니다.."), _T("Login Complete"), MB_OK);
				MY_CHECK_POINT("성공적으로 로그인되었습니다.");
				CDialogEx::OnOK();
			}
			else {
				MessageBox(_T("입력하신 Password가 일치하지 않습니다"), _T("Password Check"), MB_ICONERROR);
			}
		}
		
	}

}


void Login::OnBnClickedBtnCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PostQuitMessage(0);
}
